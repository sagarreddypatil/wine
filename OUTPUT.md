Last Session: claude --resume 0f5b5126-59c7-4b5b-9e2e-d9be1aaba6af

λ ./test.sh
======================================================
  OpenGL 4.6 Implementation Test Suite
  Built: Mar 22 2026 08:32:18
======================================================
00dc:err:vulkan:vulkan_init_once Wine was built without Vulkan support.
WARNING: Some incorrect rendering might occur because the selected Vulkan device (Apple M3 Pro) doesn't support base Zink requirements: feats.features.fillModeNonSolid have_EXT_custom_border_color have_EXT_line_rasterization 
WARNING: Some incorrect rendering might occur because the selected Vulkan device (Apple M3 Pro) doesn't support base Zink requirements: feats.features.fillModeNonSolid have_EXT_custom_border_color have_EXT_line_rasterization 

  Context created: OpenGL 4.6 Core Profile (Debug)

=== Loading GL Function Pointers ===
  Core functions missing: 0

=== Debug Output ===
  [PASS] Debug output setup

=== Basic GL Info ===
  GL_VENDOR:   Mesa
  GL_RENDERER: zink Vulkan 1.3(Apple M3 Pro (MESA_KOSMICKRISP))
  GL_VERSION:  4.6 (Core Profile) Mesa 26.0.0-devel (git-a325151787)
  GL_SHADING_LANGUAGE_VERSION: 4.60
  Reported GL version: 4.6
  [PASS] GL version >= 4.6

=== Extensions ===
  Total extensions: 202
  [PASS] Extension enumeration

=== GL 4.6 Required Features (extension checks) ===
  [PASS] GL_ARB_gl_spirv
  [PASS] GL_ARB_spirv_extensions
  [SKIP] GL_ARB_indirect_parameters - not listed (may be implicit in core 4.6context)
  [SKIP] GL_ARB_pipeline_statistics_query - not listed (may be implicit in core 4.6 context)
  [SKIP] GL_ARB_polygon_offset_clamp - not listed (may be implicit in core 4.context)
  [PASS] GL_ARB_shader_atomic_counter_ops
  [PASS] GL_ARB_shader_draw_parameters
  [PASS] GL_ARB_shader_group_vote
  [PASS] GL_ARB_texture_filter_anisotropic
  [PASS] GL_ARB_transform_feedback_overflow_query
  [PASS] GL_KHR_no_error

=== Key Implementation Limits ===
  GL_MAX_TEXTURE_SIZE                           = 16384
  GL_MAX_3D_TEXTURE_SIZE                        = 2048
  GL_MAX_CUBE_MAP_TEXTURE_SIZE                  = 16384
  GL_MAX_TEXTURE_IMAGE_UNITS                    = 32
  GL_MAX_VERTEX_ATTRIBS                         = 16
  GL_MAX_UNIFORM_BUFFER_BINDINGS                = 105
  GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS         = 40
  GL_MAX_FRAMEBUFFER_WIDTH                      = 16384
  GL_MAX_FRAMEBUFFER_HEIGHT                     = 16384
  GL_MAX_COMPUTE_WORK_GROUP_COUNT             = 2147483646, 65535, 65535
  GL_MAX_COMPUTE_WORK_GROUP_SIZE              = 1024, 1024, 1024
  GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS         = 1024
  GL_MAX_TEXTURE_MAX_ANISOTROPY               = 16.0
  [PASS] Anisotropic filtering support

=== Shader Compile + Render (GLSL 460) ===
  [PASS] GLSL 460 vertex shader compile
  [PASS] GLSL 460 fragment shader compile
  [PASS] GLSL 460 program link
  [GL DEBUG HIGH] GL_INVALID_ENUM in glBindBufferARB(target GL_FLOAT_RGB32_NV)
  [GL DEBUG HIGH] GL_INVALID_ENUM in glBufferData(target)
  [GL DEBUG HIGH] GL_INVALID_OPERATION in glVertexAttribPointer(non-VBO array)
MESA: warning: WARNING: Incorrect rendering will happen because the Vulkan device doesn't support the 'VK_EXT_depth_clip_enable' feature
  Center pixel readback: R=26 G=26 B=26 A=255
  [PASS] Triangle render + readback

=== Direct State Access (DSA) ===
  [FAIL] DSA vertex array + buffer setup - GL error generated
  [PASS] DSA texture creation + storage

=== Compute Shader ===
  [PASS] GLSL 460 compute shader compile
  [PASS] Compute program link
  [PASS] Compute shader dispatch + SSBO readback

=== SPIR-V Support (GL_ARB_gl_spirv) ===
  [PASS] glShaderBinary entry point present
  [PASS] glSpecializeShader entry point present
  Shader binary formats reported: 1
  [PASS] SPIR-V binary format listed

=== Polygon Offset Clamp (GL_ARB_polygon_offset_clamp) ===
  [GL DEBUG HIGH] GL_INVALID_OPERATION in unsupported function (glPolygonOffsetClamp) called
  [FAIL] glPolygonOffsetClamp - GL error generated

=== Anisotropic Texture Filtering (GL_ARB_texture_filter_anisotropic) ===
  Max anisotropy: 16.0
  [PASS] Set max anisotropy on texture

=== GLSL 460 Specific Features ===
  [PASS] GLSL 460 gl_HelperInvocation
  [GL DEBUG HIGH] 0:3(1): error: invalid xfb_buffer specified 0 is larger thaMAX_TRANSFORM_FEEDBACK_BUFFERS - 1 (-1).
  Shader compile error (draw-params-vs): 0:3(1): error: invalid xfb_buffer specified 0 is larger than MAX_TRANSFORM_FEEDBACK_BUFFERS - 1 (-1).

  [FAIL] GLSL 460 gl_DrawID - compile failed

======================================================
  RESULTS: 25 passed, 3 failed, 3 skipped
  GL debug errors during tests: 5
======================================================

Results also saved to gl46_test_results.txt
Window will stay open for 5 seconds...
0024:err:vulkan:vulkan_init_once Wine was built without Vulkan support.
