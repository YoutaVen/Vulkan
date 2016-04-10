/**
 * VKTS - VulKan ToolS.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) since 2014 Norbert Nopper
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef VKTS_HPP_
#define VKTS_HPP_

/**
 *
 * C++11 Libraries.
 *
 */

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

/**
 *
 * GLM Library.
 * https://www.opengl.org/sdk/libs/GLM/
 *
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 *
 * OS platform headers. Needs to be included before Vulkan headers.
 *
 */

#include <vkts/fn_os_platform.hpp>

/**
 *
 * Vulkan Library.
 * https://www.khronos.org/vulkan
 *
 */

#include <vulkan/vulkan.h>

/**
 *
 * VKTS Start.
 *
 */

/**
 * API platform.
 */

#include <vkts/fn_api_platform.hpp>

/**
 * Window platform.
 */

#include <vkts/fn_window_platform.hpp>

//

/**
 * Defines.
 */

#define VKTS_ENGINE_NAME            "VKTS"
#define VKTS_ENGINE_MAJOR           0
#define VKTS_ENGINE_MINOR           9
#define VKTS_ENGINE_PATCH           0

#define VKTS_PI                     3.1415926535897932384626433832795f

#define VKTS_DEFAULT_WINDOW_INDEX   0

#include <vkts/layer0/input/fn_input_gamepad.hpp>
#include <vkts/layer0/input/fn_input_key.hpp>
#include <vkts/layer0/input/fn_input_mouse.hpp>

/**
 * Types.
 */

#include <vkts/fn_types.hpp>

/**
 * Bindings.
 */

#include <vkts/fn_bindings.hpp>

//
// Layer 0
//

/**
 * Base.
 */

#include <vkts/layer0/base/ICloneable.hpp>
#include <vkts/layer0/base/IDestroyable.hpp>
#include <vkts/layer0/base/IResetable.hpp>
#include <vkts/layer0/base/IUpdateable.hpp>
#include <vkts/layer0/base/SmartPointerList.hpp>
#include <vkts/layer0/base/SmartPointerVector.hpp>
#include <vkts/layer0/base/ThreadsafeQueue.hpp>
#include <vkts/layer0/base/Vector.hpp>

#include <vkts/layer0/base/Map.hpp>
#include <vkts/layer0/base/SmartPointerMap.hpp>

/**
 * Log.
 */

#include <vkts/layer0/log/fn_log.hpp>

/**
 * Extension.
 */

#include <vkts/layer0/extension/fn_extension.hpp>

/**
 * Time.
 */

#include <vkts/layer0/time/fn_time.hpp>

/**
 * Barrier.
 */

#include <vkts/layer0/barrier/fn_barrier.hpp>

/**
 * Processor.
 */

#include <vkts/layer0/processor/fn_processor.hpp>

/**
 * Visual.
 */

#include <vkts/layer0/visual/INativeDisplay.hpp>
#include <vkts/layer0/visual/INativeWindow.hpp>

#include <vkts/layer0/visual/fn_visual.hpp>

/**
 * Engine.
 */

#include <vkts/layer0/engine/ITask.hpp>
#include <vkts/layer0/engine/IUpdateThreadContext.hpp>
#include <vkts/layer0/engine/IUpdateThread.hpp>

#include <vkts/layer0/engine/fn_engine.hpp>

/**
 * Binary buffer.
 */

#include <vkts/layer0/binary_buffer/IBinaryBuffer.hpp>

#include <vkts/layer0/binary_buffer/fn_binary_buffer.hpp>

/**
 * Text buffer.
 */

#include <vkts/layer0/text_buffer/ITextBuffer.hpp>

#include <vkts/layer0/text_buffer/fn_text_buffer.hpp>

/**
 * File.
 */

#include <vkts/layer0/file/fn_file.hpp>

/**
 * Common.
 */

#include <vkts/layer0/common/fn_common.hpp>

/**
 * Matrix.
 */

#include <vkts/layer0/matrix/fn_matrix.hpp>
#include <vkts/layer0/matrix/fn_matrix_viewprojection.hpp>

/**
 * Geometry.
 */

#include <vkts/layer0/geometry/plane.hpp>
#include <vkts/layer0/geometry/sphere.hpp>

/**
 * Intersect.
 */

#include <vkts/layer0/intersect/obb.hpp>
#include <vkts/layer0/intersect/aabb.hpp>

/**
 * Cull.
 */

#include <vkts/layer0/cull/frustum.hpp>

/**
 * Quaternion.
 */

#include <vkts/layer0/quaternion/quat.hpp>

#include <vkts/layer0/quaternion/fn_quat.hpp>

/**
 * Instance.
 */

#include <vkts/layer0/instance/IInstance.hpp>

#include <vkts/layer0/instance/fn_instance.hpp>

/**
 * Physical device.
 */

#include <vkts/layer0/physical_device/IPhysicalDevice.hpp>

#include <vkts/layer0/physical_device/fn_physical_device.hpp>

/**
 * Device.
 */

#include <vkts/layer0/device/IDevice.hpp>

#include <vkts/layer0/device/fn_device.hpp>

/**
 * Queue.
 */

#include <vkts/layer0/queue/IQueue.hpp>

#include <vkts/layer0/queue/fn_queue.hpp>

/**
 * Command.
 */

#include <vkts/layer0/command/ICommandBuffers.hpp>
#include <vkts/layer0/command/ICommandPool.hpp>

#include <vkts/layer0/command/fn_command.hpp>

/**
 * Fence.
 */

#include <vkts/layer0/fence/IFence.hpp>

#include <vkts/layer0/fence/fn_fence.hpp>

/**
 * Semaphore.
 */

#include <vkts/layer0/semaphore/ISemaphore.hpp>

#include <vkts/layer0/semaphore/fn_semaphore.hpp>

/**
 * Shader module.
 */

#include <vkts/layer0/shader_module/IShaderModule.hpp>

#include <vkts/layer0/shader_module/fn_shader_module.hpp>

/**
 * Buffer.
 */

#include <vkts/layer0/buffer/IBuffer.hpp>
#include <vkts/layer0/buffer/IBufferView.hpp>

#include <vkts/layer0/buffer/fn_buffer.hpp>

/**
 * Image data.
 */

#include <vkts/layer0/image_data/IImageData.hpp>

#include <vkts/layer0/image_data/fn_image_data.hpp>

/**
 * Image.
 */

#include <vkts/layer0/image/IImage.hpp>

#include <vkts/layer0/image/fn_image.hpp>

/**
 * Image view.
 */

#include <vkts/layer0/image_view/IImageView.hpp>

#include <vkts/layer0/image_view/fn_image_view.hpp>

/**
 * Sampler.
 */

#include <vkts/layer0/sampler/ISampler.hpp>

#include <vkts/layer0/sampler/fn_sampler.hpp>

/**
 * Device memory.
 */

#include <vkts/layer0/device_memory/IDeviceMemory.hpp>

#include <vkts/layer0/device_memory/fn_device_memory.hpp>

/**
 * Descriptor.
 */

#include <vkts/layer0/descriptor/IDescriptorSetLayout.hpp>
#include <vkts/layer0/descriptor/IDescriptorPool.hpp>
#include <vkts/layer0/descriptor/IDescriptorSets.hpp>

#include <vkts/layer0/descriptor/fn_descriptor.hpp>

/**
 * Render pass.
 */

#include <vkts/layer0/render_pass/IRenderPass.hpp>

#include <vkts/layer0/render_pass/fn_render_pass.hpp>

/**
 * Frame buffer.
 */

#include <vkts/layer0/framebuffer/IFramebuffer.hpp>

#include <vkts/layer0/framebuffer/fn_framebuffer.hpp>

/**
 * Pipeline.
 */

#include <vkts/layer0/pipeline/IComputePipeline.hpp>
#include <vkts/layer0/pipeline/IGraphicsPipeline.hpp>
#include <vkts/layer0/pipeline/IPipelineCache.hpp>
#include <vkts/layer0/pipeline/IPipelineLayout.hpp>

#include <vkts/layer0/pipeline/fn_pipeline.hpp>

/**
 * Debug EXT.
 */

#include <vkts/layer0/debug/fn_debug.hpp>

/**
 * KHR.
 */

#if !defined(VKTS_NO_VISUAL)

#include <vkts/layer0/wsi/ISurface.hpp>
#include <vkts/layer0/wsi/ISwapchain.hpp>

#include <vkts/layer0/wsi/fn_wsi.hpp>

#endif

//
// Layer 1
//

/**
 * Base.
 */

#include <vkts/layer1/base/IMoveable.hpp>

/**
 * Initial resources.
 */

#include <vkts/layer1/initial_resources/IInitialResources.hpp>

#include <vkts/layer1/initial_resources/fn_initial_resources.hpp>

/**
 * Buffer object.
 */

#include <vkts/layer1/buffer_object/IBufferObject.hpp>

#include <vkts/layer1/buffer_object/fn_buffer_object.hpp>

/**
 * Camera.
 */

#include <vkts/layer1/camera/ICamera.hpp>

#include <vkts/layer1/camera/fn_camera.hpp>

/**
 * Input controller.
 */

#include <vkts/layer1/input_controller/IInputController.hpp>

#include <vkts/layer1/input_controller/fn_input_controller.hpp>

/**
 * Memory Image.
 */

#include <vkts/layer1/memory_image/IMemoryImage.hpp>

#include <vkts/layer1/memory_image/fn_memory_image.hpp>

/**
 * Texture.
 */

#include <vkts/layer1/texture/ITexture.hpp>

#include <vkts/layer1/texture/fn_texture.hpp>

/**
 * Scenegraph.
 */

#include <vkts/layer1/scenegraph/IChannel.hpp>

#include <vkts/layer1/scenegraph/IBSDFMaterial.hpp>

#include <vkts/layer1/scenegraph/IPhongMaterial.hpp>

#include <vkts/layer1/scenegraph/ISubMesh.hpp>

#include <vkts/layer1/scenegraph/IAnimation.hpp>

#include <vkts/layer1/scenegraph/IMesh.hpp>

#include <vkts/layer1/scenegraph/INode.hpp>

#include <vkts/layer1/scenegraph/IObject.hpp>

#include <vkts/layer1/scenegraph/IScene.hpp>

#include <vkts/layer1/scenegraph/IContext.hpp>

#include <vkts/layer1/scenegraph/fn_scenegraph.hpp>

/**
 * Render overwrites.
 */

#include <vkts/layer1/overwrite/overwrite.hpp>

#include <vkts/layer1/overwrite/blend.hpp>
#include <vkts/layer1/overwrite/cull.hpp>
#include <vkts/layer1/overwrite/displace.hpp>

/**
 * Interpolator.
 */

#include <vkts/layer1/interpolator/fn_interpolator.hpp>

#endif /* VKTS_HPP_ */
