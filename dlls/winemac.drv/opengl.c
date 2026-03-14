/*
 * Mac driver OpenGL support via EGL (surfaceless platform + Zink/KosmicKrisp)
 *
 * Copyright 2012 Alexandre Julliard
 * Copyright 2012, 2013 Ken Thomases for CodeWeavers Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#if 0
#pragma makedep unix
#endif

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "ntstatus.h"
#include "macdrv.h"
#include "wine/debug.h"
#include "wine/opengl_driver.h"

WINE_DEFAULT_DEBUG_CHANNEL(wgl);

static const struct egl_platform *egl;
static const struct opengl_funcs *funcs;
static const struct opengl_drawable_funcs macdrv_drawable_funcs;

struct macdrv_gl_drawable
{
    struct opengl_drawable base;
};

static EGLConfig egl_config_for_format(int format)
{
    return egl->configs[(format - 1) % egl->config_count];
}

/* Find an EGL config with EGL_WINDOW_BIT for the given format. The format's
 * default config may be pbuffer-only (10-bit configs lack window support in
 * mesa's surfaceless platform). We find the best window-capable match. */
static EGLConfig find_window_config(int format)
{
    EGLConfig config = egl_config_for_format(format);
    EGLConfig best = 0;
    EGLint surface_type, depth, stencil, best_score = -1;

    funcs->p_eglGetConfigAttrib(egl->display, config, EGL_SURFACE_TYPE, &surface_type);
    if (surface_type & EGL_WINDOW_BIT) return config;

    funcs->p_eglGetConfigAttrib(egl->display, config, EGL_DEPTH_SIZE, &depth);
    funcs->p_eglGetConfigAttrib(egl->display, config, EGL_STENCIL_SIZE, &stencil);

    for (int i = 0; i < egl->config_count; i++)
    {
        EGLint type, cd, cs, cr, score;
        funcs->p_eglGetConfigAttrib(egl->display, egl->configs[i], EGL_SURFACE_TYPE, &type);
        if (!(type & EGL_WINDOW_BIT)) continue;

        /* Score: prefer matching depth/stencil and higher color depth */
        funcs->p_eglGetConfigAttrib(egl->display, egl->configs[i], EGL_DEPTH_SIZE, &cd);
        funcs->p_eglGetConfigAttrib(egl->display, egl->configs[i], EGL_STENCIL_SIZE, &cs);
        funcs->p_eglGetConfigAttrib(egl->display, egl->configs[i], EGL_RED_SIZE, &cr);
        score = cr; /* prefer higher color depth */
        if (cd >= depth) score += 100; /* bonus for sufficient depth */
        if (cs >= stencil) score += 50; /* bonus for sufficient stencil */

        if (score > best_score)
        {
            best_score = score;
            best = egl->configs[i];
        }
    }

    if (best)
    {
        TRACE("format %d: using window-capable config %p (score %d)\n", format, best, best_score);
        return best;
    }

    WARN("No window-capable EGL config found for format %d\n", format);
    return config;
}

static void macdrv_drawable_destroy(struct opengl_drawable *base)
{
    TRACE("%s\n", debugstr_opengl_drawable(base));
}

static void macdrv_drawable_flush(struct opengl_drawable *base, UINT flags)
{
    TRACE("drawable %s, flags %#x\n", debugstr_opengl_drawable(base), flags);

    if (flags & GL_FLUSH_INTERVAL)
        funcs->p_eglSwapInterval(egl->display, abs(base->interval));
}

static BOOL macdrv_drawable_swap(struct opengl_drawable *base)
{
    TRACE("drawable %s\n", debugstr_opengl_drawable(base));

    client_surface_present(base->client);
    funcs->p_eglSwapBuffers(egl->display, base->surface);
    return TRUE;
}

static BOOL macdrv_surface_create(HWND hwnd, int format, struct opengl_drawable **drawable)
{
    EGLConfig config = find_window_config(format);
    struct macdrv_client_surface *client;
    struct opengl_drawable *previous;
    struct macdrv_gl_drawable *gl;

    TRACE("hwnd=%p format=%d\n", hwnd, format);

    if ((previous = *drawable) && previous->format == format) return TRUE;

    if (!(client = macdrv_client_surface_create(hwnd))) return FALSE;

    /* Create a Metal device and WineMetalView (with a proper CAMetalLayer),
     * mirroring what the Vulkan path does. This gives Mesa's surfaceless
     * platform a real CAMetalLayer for Zink rendering via KosmicKrisp. */
    if (!(client->metal_device = macdrv_create_metal_device()))
    {
        ERR("Failed to create Metal device for EGL surface\n");
        client_surface_release(&client->client);
        return FALSE;
    }
    if (!(client->metal_view = macdrv_view_create_metal_view(client->cocoa_view, client->metal_device)))
    {
        ERR("Failed to create Metal view for EGL surface\n");
        client_surface_release(&client->client);
        return FALSE;
    }

    gl = opengl_drawable_create(sizeof(*gl), &macdrv_drawable_funcs, format, &client->client);
    client_surface_release(&client->client);
    if (!gl) return FALSE;

    gl->base.buffer_map[0] = GL_BACK_LEFT;
    gl->base.buffer_map[1] = GL_BACK_RIGHT;
    gl->base.buffer_map[GL_FRONT - GL_FRONT_LEFT] = GL_BACK;
    gl->base.buffer_map[GL_FRONT_AND_BACK - GL_FRONT_LEFT] = GL_BACK;

    /* Pass the CAMetalLayer from the WineMetalView to EGL. */
    if (!(gl->base.surface = funcs->p_eglCreateWindowSurface(egl->display, config,
                                                              macdrv_view_get_metal_layer(client->metal_view), NULL)))
    {
        ERR("eglCreateWindowSurface failed\n");
        goto err;
    }

    TRACE("Created drawable %s with egl_surface %p\n", debugstr_opengl_drawable(&gl->base), gl->base.surface);

    if (previous) opengl_drawable_release(previous);
    *drawable = &gl->base;
    return TRUE;

err:
    opengl_drawable_release(&gl->base);
    return FALSE;
}

static void macdrv_init_egl_platform(struct egl_platform *platform)
{
    platform->type = EGL_PLATFORM_SURFACELESS_MESA;
    platform->native_display = 0;
    platform->force_pbuffer_formats = TRUE;
    egl = platform;
}

static struct opengl_driver_funcs macdrv_driver_funcs =
{
    .p_init_egl_platform = macdrv_init_egl_platform,
    .p_surface_create = macdrv_surface_create,
};

static const struct opengl_drawable_funcs macdrv_drawable_funcs =
{
    .destroy = macdrv_drawable_destroy,
    .flush = macdrv_drawable_flush,
    .swap = macdrv_drawable_swap,
};

/**********************************************************************
 *              macdrv_OpenGLInit
 */
UINT macdrv_OpenGLInit(UINT version, const struct opengl_funcs *opengl_funcs, const struct opengl_driver_funcs **driver_funcs)
{
    TRACE("()\n");

    if (version != WINE_OPENGL_DRIVER_VERSION)
    {
        ERR("version mismatch, opengl32 wants %u but macdrv has %u\n", version, WINE_OPENGL_DRIVER_VERSION);
        return STATUS_INVALID_PARAMETER;
    }

    if (!opengl_funcs->egl_handle) return STATUS_NOT_SUPPORTED;
    funcs = opengl_funcs;

    /* Copy EGL default implementations for functions we don't override */
    macdrv_driver_funcs.p_get_proc_address = (*driver_funcs)->p_get_proc_address;
    macdrv_driver_funcs.p_init_pixel_formats = (*driver_funcs)->p_init_pixel_formats;
    macdrv_driver_funcs.p_describe_pixel_format = (*driver_funcs)->p_describe_pixel_format;
    macdrv_driver_funcs.p_init_wgl_extensions = (*driver_funcs)->p_init_wgl_extensions;
    macdrv_driver_funcs.p_context_create = (*driver_funcs)->p_context_create;
    macdrv_driver_funcs.p_context_destroy = (*driver_funcs)->p_context_destroy;
    macdrv_driver_funcs.p_make_current = (*driver_funcs)->p_make_current;
    macdrv_driver_funcs.p_pbuffer_create = (*driver_funcs)->p_pbuffer_create;
    macdrv_driver_funcs.p_pbuffer_updated = (*driver_funcs)->p_pbuffer_updated;
    macdrv_driver_funcs.p_pbuffer_bind = (*driver_funcs)->p_pbuffer_bind;

    *driver_funcs = &macdrv_driver_funcs;
    return STATUS_SUCCESS;
}
