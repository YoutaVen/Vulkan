/**
 * VKTS Examples - Examples for Vulkan using VulKan ToolS.
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

#ifndef EXAMPLE_HPP_
#define EXAMPLE_HPP_

#include <vkts/vkts.hpp>

#define VKTS_EXAMPLE_NAME "Example03"

#define VKTS_NUMBER_DYNAMIC_STATES 2

#define VKTS_NUMBER_BUFFERS 2
#define VKTS_SHADER_STAGE_COUNT 2
#define VKTS_PIPELINE_CACHE_SIZE 0

#define VKTS_VERTEX_SHADER_NAME "shader/SPIR/V/texture_ndc.vert.spv"
#define VKTS_FRAGMENT_SHADER_NAME "shader/SPIR/V/texture.frag.spv"

#define VKTS_TEXTURE_NAME "texture/crater_lake.tga"

class Example: public vkts::IUpdateThread
{

private:

	const vkts::IInstanceSP instance;

	const vkts::IPhysicalDeviceSP physicalDevice;

	const int32_t windowIndex;

	const vkts::ISurfaceSP surface;

	const vkts::IDeviceSP device;

	const vkts::IQueueSP queue;

	vkts::ICommandPoolSP commandPool;

    vkts::ISemaphoreSP imageAcquiredSemaphore;
    vkts::ISemaphoreSP renderingCompleteSemaphore;

	vkts::IImageSP image;
	vkts::IDeviceMemorySP deviceMemoryImage;
	vkts::ISamplerSP sampler;
	vkts::IImageViewSP imageView;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;

	vkts::IBufferSP vertexBuffer;
	vkts::IDeviceMemorySP deviceMemoryVertexBuffer;

	vkts::IShaderModuleSP vertexShaderModule;
	vkts::IShaderModuleSP fragmentShaderModule;

	vkts::IPipelineLayoutSP pipelineLayout;

	vkts::ISwapchainSP swapchain;

	vkts::IRenderPassSP renderPass;

	vkts::IGraphicsPipelineSP pipeline;

    uint32_t swapchainImagesCount;

    vkts::SmartPointerVector<vkts::IImageViewSP> swapchainImageView;

    vkts::SmartPointerVector<vkts::IFramebufferSP> framebuffer;

    vkts::SmartPointerVector<vkts::ICommandBuffersSP> cmdBuffer;

	VkBool32 createTexture(vkts::IImageSP& currentImage, vkts::IDeviceMemorySP& currentDeviceMemoryImage, const vkts::IImageDataSP& imageData, const VkImageTiling imageTiling, const VkImageUsageFlags usage, const VkImageLayout initialLayout, const VkMemoryPropertyFlags memoryPropertyFlagBits, const VkAccessFlags accessMask) const;

	VkBool32 destroyTexture(vkts::IImageSP& currentImage, vkts::IDeviceMemorySP& currentDeviceMemoryImage) const;

	//

	VkBool32 buildCmdBuffer(const int32_t usedBuffer);

	VkBool32 buildFramebuffer(const int32_t usedBuffer);

	VkBool32 buildSwapchainImageView(const int32_t usedBuffer);

	VkBool32 updateDescriptorSets();

	VkBool32 buildTexture(const vkts::ICommandBuffersSP& cmdBuffer, vkts::IImageSP& stageImage, vkts::IDeviceMemorySP& stageDeviceMemoryImage);

	VkBool32 buildPipeline();

	VkBool32 buildRenderPass();

	VkBool32 buildPipelineLayout();

	VkBool32 buildDescriptorSets();

	VkBool32 buildDescriptorSetPool();

	VkBool32 buildDescriptorSetLayout();

	VkBool32 buildShader();

	VkBool32 buildVertexBuffer();

	VkBool32 buildResources(const vkts::IUpdateThreadContext& updateContext);

	void terminateResources(const vkts::IUpdateThreadContext& updateContext);

public:

	Example(const vkts::IInstanceSP& instance, const vkts::IPhysicalDeviceSP& physicalDevice, const int32_t windowIndex, const vkts::ISurfaceSP& surface, const vkts::IDeviceSP& device, const vkts::IQueueSP& queue);

	virtual ~Example();

	//
	// Vulkan initialization.
	//
	virtual VkBool32 init(const vkts::IUpdateThreadContext& updateContext);

	//
	// Vulkan update.
	//
	virtual VkBool32 update(const vkts::IUpdateThreadContext& updateContext);

	//
	// Vulkan termination.
	//
	virtual void terminate(const vkts::IUpdateThreadContext& updateContext);

};

#endif /* EXAMPLE_HPP_ */
