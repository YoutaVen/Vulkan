VulKan ToolS (VKTS):
--------------------

Authors:

**Norbert Nopper** (norbert@nopper.tv)

VKTS version: **0.9.0**

Depending Vulkan API header version: **1.0.8**

Depending Vulkan extensions, when using a window system:

- `VK_KHR_swapchain`
- `VK_KHR_surface`

- Linux X11: `VK_KHR_xlib_surface` or
             `VK_KHR_xcb_surface`
- Android:   `VK_KHR_android_surface`
- Windows:   `VK_KHR_win32_surface`


About:
------

VulKan ToolS (VKTS) is a cross platform, C++ 11 helper library for [Vulkan](https://www.khronos.org/vulkan) released under the [MIT License](VKTS_license.txt).


Used libraries (beside C++ 11):
-------------------------------

- [LunarG Vulkan SDK](http://vulkan.lunarg.com)
- [GLM](http://glm.g-truc.net)


Supported platforms:
--------------------

- Linux X11 or console only (GNU GCC)
- Android (GNU GCC) 
- Microsoft Windows or console only (GNU MinGW & Visual C++ 2015)


Features:
---------

- Platform abstraction.
- Logging functions.
- Matrix, plane, sphere and quaternion classes and functions.
- Vulkan wrapper objects and factory functions.
- Basic multi threaded engine including synchronization, time and CPU querying.   
- Basic native display handling including multi display support.
- Basic native window handling including fullscreen support.
- Basic input handling: Gamepad, keyboard and mouse.
- None visual configuration for console/compute applications.
- Loading and saving of text, binary and image files (TGA and HDR).
- VKTS scene file format:
    - 3D objects (incl. node hierarchy)
    - Meshes (incl. sub meshes)
    - Materials (Phong and BSDF)
    - Textures (TGA and HDR)
    - Animations (incl. node and armature)
- Blender Python exporter for the VKTS scene file format.
- GLSL to SPIR-V build script.
- Interpolator and converter for constant, linear and bezier key frame data.


How to build:
-------------

- [System requirements and step by step tutorial](/VKTS_Documentation/BUILD.md)


Changelog:
----------

04/09/2016 - Split up textures into device images, having sampler and view.
           - Optimized material management, by avoiding unnecessary clone.

04/07/2016 - Refactored overwrite visitor to be more flexible.
		   - Fixed frustum culling.
		   - Fixed plane class.
		   - Fixed bounding sphere calcualtion.
		   - Updated to LunarG SDK 1.8.0. 

04/06/2016 - Fixed camera creation.

04/05/2016 - Fixed bug in Blender export script.
		   - Increased max joints to 96. Added error output, if too many joints are used.

04/04/2016 - Added physical device features query.
           - Added blend overwrite.

04/01/2016 - Fixed camera position creation.
           - Fixed animation duration in Blender exporter.
           - Improved Blender Cycles material exporter.

03/30/2016 - Refactored code for having BSDF shader.
           - Added first version of Blender Cycles material exporter.

03/29/2016 - Fixed issue, that mesh was rendered once when shared with other nodes.

03/28/2016 - Multiple graphics pipelines can be past to the scene, which makes it more robust and flexible.

03/27/2016 - Fixed bezier interpolator regarding infinite loop.

03/26/2016 - Added wrapper class for compute pipeline.
           - Improved file saving on Android, as file does not have to exist.
           - Added possibility to overwrite scene rendering behaviour.
           - Added to scene file, if transparency is enabled or not.

03/25/2016 - Fixes, that VKTS can be build without any windowing system.
           - Enhanced fence wrapper class.

03/24/2016 - Added linux mouse handler for devices using VK_KHR_display extension.

03/23/2016 - Added linux keyboard handler for devices using VK_KHR_display extension.

03/22/2016 - Improved Android file access wrapper classes.
           - Using VK_PRESENT_MODE_FIFO_KHR for present mode as default. 

03/21/2016 - Removed legacy defines.
           - Using Vulkan prototypes for extensions.
           - Added 32 bit CMake build configurations.
           - Added initial support for VK_KHR_display extension.

03/19/2016 - Improved wrapper class for initial resources.

03/17/2016 - Added view frustum culling.
           - Scene file allows optional definition of displacement.
           - Scene file allows optional definition of AABB.           

03/16/2016 - Continued on aabb and sphere class.

03/15/2016 - Continued on classes for culling and intersecting test.
           - Using current extent of surface capabilities for swapchain creation.

03/14/2016 - Added frustum class for doing frustum culling.
           - Added first version of oriented and axis aligned bounding boxes.
           - Add changes for Android NDK r11, which support Vulkan:
              - Using recommended clang now. Removed detected warnings by clang.
              - Using platform android-24.
           
03/12/2016 - Updated to Vulkan API header version 1.0.5 and using LunarG Vulkan SDK 1.0.5.0.

03/11/2016 - Added general extension handler.
           - Added debug reporting.

03/10/2016 - Added plane and sphere class for distance calculations.

03/09/2016 - Switched to XInput version 9.1.0 on Windows.

03/08/2016 - Improved/fixed task queue by limiting elements and avoiding "infinite" heap grow.
           - Improved scenegraph for parallel execution.

03/05/2016 - Improved/fixed Linux gamepad detection code.

03/04/2016 - Fixed swap chain creation, that actual number of images is used. 
           - Improved smart pointer list, map and vector.

03/03/2016 - Made command buffers resetable.

03/02/2016 - Fixed keyboard lookup table, which did write out of bounds.

02/28/2016 - Added layout qualifier to all shaders.
           - Fixed projection matrices, that z goes from 0 to 1.
           - Removed invalid access flags.

02/24/2016 - Retested for AMD, Intel and NVIDIA GPUs and removed all layer warnings.
           - Fixed task infinite wait in case execution thread was faster.
           - Improved scene class, that only used resources can be bound.

02/23/2016 - Fixed and improved the task executor engine.

02/21/2016 - Updated to GLM 0.9.7.3.
           - Extended file load and save functionality for Android devices.

02/19/2016 - Refactored physical device wrapper class and added new query function.
           - Under Linux, the VK_KHR_xcb_surface extension is now preferred.

02/16/2016 - First public release.
