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

#define VKTS_EXAMPLE_NAME "Example08"

#define VKTS_IMAGE_NAME "texture/mandelbrot.tga"

#define VKTS_COMPUTE_SHADER_NAME "shader/SPIR/V/mandelbrot.comp.spv"

// Has to be the same as in compute shader.
#define VKTS_IMAGE_LENGTH	1024
#define VKTS_LOCAL_SIZE		16

class Example: public vkts::IUpdateThread
{

private:

	const vkts::IInstanceSP instance;

	const vkts::IPhysicalDeviceSP physicalDevice;

	const vkts::IDeviceSP device;

	const vkts::IQueueSP queue;

	vkts::IFenceSP fence;

	vkts::IShaderModuleSP computeShaderModule;

	vkts::ICommandPoolSP commandPool;

	vkts::IImageSP image;
	vkts::IDeviceMemorySP deviceMemoryImage;
	vkts::IImageViewSP imageView;

	vkts::IDescriptorSetLayoutSP descriptorSetLayout;
	vkts::IDescriptorPoolSP descriptorPool;
	vkts::IDescriptorSetsSP descriptorSet;

	vkts::IPipelineLayoutSP pipelineLayout;
	vkts::IComputePipelineSP pipeline;

	vkts::ICommandBuffersSP cmdBuffer;

	vkts::IImageDataSP gatherImageData() const;

	VkBool32 buildCmdBuffer();

	VkBool32 buildPipeline();

	VkBool32 buildPipelineLayout();

	VkBool32 buildDescriptorSets();

	VkBool32 buildDescriptorSetPool();

	VkBool32 buildDescriptorSetLayout();

	VkBool32 createBuffer(vkts::IBufferSP& buffer, vkts::IDeviceMemorySP& deviceMemory, const VkBufferCreateInfo& bufferCreateInfo, const VkMemoryPropertyFlags memoryPropertyFlags) const;

	VkBool32 createTexture(vkts::IImageSP& currentImage, vkts::IDeviceMemorySP& currentDeviceMemoryImage, const VkImageTiling imageTiling, const VkImageUsageFlags usage, const VkImageLayout initialLayout, const VkMemoryPropertyFlags memoryPropertyFlagBits, const VkAccessFlags accessMask) const;

	VkBool32 destroyTexture(vkts::IImageSP& currentImage, vkts::IDeviceMemorySP& currentDeviceMemoryImage) const;

	VkBool32 buildTexture();

	VkBool32 buildCmdPool();

	VkBool32 buildShader();

public:

	Example(const vkts::IInstanceSP& instance, const vkts::IPhysicalDeviceSP& physicalDevice, const vkts::IDeviceSP& device, const vkts::IQueueSP& queue);

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
