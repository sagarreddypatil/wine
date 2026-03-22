GLXGears version: WORKS LETS GOO

but for the GLSLTestbed.exe, it opens window named PK Renderer, but it's empty (black)
```
00e0:err:vulkan:vulkan_init_once Wine was built without Vulkan support.
WARNING: Some incorrect rendering might occur because the selected Vulkan device (Apple M3 Pro) doesn't support base Zink requirements: feats.features.fillModeNonSolid have_EXT_custom_border_color have_EXT_line_rasterization 
GLFW Initialized
Creating window PK Renderer (1024, 512)
0024:fixme:msg:ChangeWindowMessageFilterEx 000000000001005E 233 1 0000000000000000
0024:fixme:msg:ChangeWindowMessageFilterEx 000000000001005E 4a 1 0000000000000000
0024:fixme:msg:ChangeWindowMessageFilterEx 000000000001005E 49 1 0000000000000000
WARNING: Some incorrect rendering might occur because the selected Vulkan device (Apple M3 Pro) doesn't support base Zink requirements: feats.features.fillModeNonSolid have_EXT_custom_border_color have_EXT_line_rasterization 
0024:err:vulkan:vulkan_init_once Wine was built without Vulkan support.
Shader (res/shaders/CS_SceneGI_Bake_Checkerboard.shader) Compilation Failure!
0:628(6): error: no matching function for call to `any(bool)'; candidates are:
0:628(6): error:    bool any(bvec2)
0:628(6): error:    bool any(bvec3)
0:628(6): error:    bool any(bvec4)
0:628(6): error: if-statement condition must be scalar boolean

0024:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 0024, blocked by 0000, retrying (60 sec)
012c:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 012c, blocked by 0000, retrying (60 sec)
0130:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 0130, blocked by 0000, retrying (60 sec)
0134:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 0134, blocked by 0000, retrying (60 sec)
0138:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 0138, blocked by 0000, retrying (60 sec)
013c:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 013c, blocked by 0000, retrying (60 sec)
0140:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 0140, blocked by 0000, retrying (60 sec)
0144:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 0144, blocked by 0000, retrying (60 sec)
0148:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 0148, blocked by 0000, retrying (60 sec)
014c:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 014c, blocked by 0000, retrying (60 sec)
0150:err:sync:RtlpWaitForCriticalSection section 00006FFFFFC591A0 "dlls/ntdll/exception.c: vectored_handlers_section" wait timed out in thread 0150, blocked by 0000, retrying (60 sec)
./test.sh: line 13: 40777 Killed: 9               ~/wine-custom/bin/wine GLSLTestbed.exe

```

and it's not ctrl-c'able, had to kill