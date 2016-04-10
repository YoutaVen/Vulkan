VulKan ToolS (VKTS) Binaries:
-----------------------------

This folder and sub folders contains all executables, shaders and assets. The examples are copied  
into this folder after the build. This folder has to be the working directory, otherwise shaders  
and assets are not found.

Using Android, the needed asset files will be copied in the asset folder after executing `Build_Install.bat`.

Inside `shader/GLSL/` there are two build scripts for the host systems, which do compile all available GLSL source files.  
The resulting SPIR-V binaries are deployed into `shader/SPIR/V`.  

To enable the standard set of Vulkan validation layers, set the following environment variables:

Linux:
export VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_standard_validation
export VK_DEVICE_LAYERS=VK_LAYER_LUNARG_standard_validation

Windows:
set VK_INSTANCE_LAYERS=VK_LAYER_LUNARG_standard_validation
set VK_DEVICE_LAYERS=VK_LAYER_LUNARG_standard_validation


Used libraries:
---------------

- [LunarG Vulkan SDK](http://vulkan.lunarg.com) (see [License](/VKTS/LunarG_license.html))