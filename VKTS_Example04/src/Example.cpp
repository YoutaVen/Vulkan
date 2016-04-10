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

#include "Example.hpp"

Example::Example(const vkts::IInitialResourcesSP& initialResources, const int32_t windowIndex, const vkts::ISurfaceSP& surface) :
		IUpdateThread(), initialResources(initialResources), windowIndex(windowIndex), surface(surface), windowDimension(0, 0), commandPool(nullptr), imageAcquiredSemaphore(nullptr), renderingCompleteSemaphore(nullptr), descriptorSetLayout(nullptr), vertexViewProjectionUniformBuffer(nullptr), fragmentUniformBuffer(nullptr), vertexShaderModule(nullptr), fragmentShaderModule(nullptr), pipelineLayout(nullptr), sceneContext(nullptr), scene(nullptr), swapchain(nullptr), renderPass(nullptr), allGraphicsPipelines(), depthTexture(nullptr), depthStencilImageView(nullptr), swapchainImagesCount(0), swapchainImageView(), framebuffer(), cmdBuffer()
{
}

Example::~Example()
{
}

VkBool32 Example::buildCmdBuffer(const int32_t usedBuffer)
{
	VkResult result;

	cmdBuffer[usedBuffer] = vkts::commandBuffersCreate(initialResources->getDevice()->getDevice(), commandPool->getCmdPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

	if (!cmdBuffer[usedBuffer].get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create command buffer.");

		return VK_FALSE;
	}

	result = cmdBuffer[usedBuffer]->beginCommandBuffer(0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, VK_FALSE, 0, 0);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not begin command buffer.");

		return VK_FALSE;
	}

    //

    swapchain->cmdPipelineBarrier(cmdBuffer[usedBuffer]->getCommandBuffer(), VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, usedBuffer);

    //

	VkClearColorValue clearColorValue;

	memset(&clearColorValue, 0, sizeof(VkClearColorValue));

	clearColorValue.float32[0] = 0.2f;
	clearColorValue.float32[1] = 0.2f;
	clearColorValue.float32[2] = 0.2f;
	clearColorValue.float32[3] = 1.0f;

	VkClearDepthStencilValue clearDepthStencilValue;

	memset(&clearDepthStencilValue, 0, sizeof(VkClearDepthStencilValue));

	clearDepthStencilValue.depth = 1.0f;
	clearDepthStencilValue.stencil = 0;

	VkClearValue clearValues[2];

	memset(clearValues, 0, sizeof(clearValues));

	clearValues[0].color = clearColorValue;
	clearValues[1].depthStencil = clearDepthStencilValue;

	VkRenderPassBeginInfo renderPassBeginInfo;

	memset(&renderPassBeginInfo, 0, sizeof(VkRenderPassBeginInfo));

	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

	renderPassBeginInfo.renderPass = renderPass->getRenderPass();
	renderPassBeginInfo.framebuffer = framebuffer[usedBuffer]->getFramebuffer();
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent = swapchain->getImageExtent();
	renderPassBeginInfo.clearValueCount = 2;
	renderPassBeginInfo.pClearValues = clearValues;

	cmdBuffer[usedBuffer]->cmdBeginRenderPass(&renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport;
	memset(&viewport, 0, sizeof(VkViewport));

	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapchain->getImageExtent().width;
	viewport.height = (float) swapchain->getImageExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	vkCmdSetViewport(cmdBuffer[usedBuffer]->getCommandBuffer(), 0, 1, &viewport);

	VkRect2D scissor;
	memset(&scissor, 0, sizeof(VkRect2D));

	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent = swapchain->getImageExtent();

	vkCmdSetScissor(cmdBuffer[usedBuffer]->getCommandBuffer(), 0, 1, &scissor);

	if (scene.get())
	{
		scene->bindDrawIndexedRecursive(cmdBuffer[usedBuffer], allGraphicsPipelines);
	}

	cmdBuffer[usedBuffer]->cmdEndRenderPass();

    //

    swapchain->cmdPipelineBarrier(cmdBuffer[usedBuffer]->getCommandBuffer(), VK_ACCESS_MEMORY_READ_BIT, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, usedBuffer);

    //

	result = cmdBuffer[usedBuffer]->endCommandBuffer();

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not end command buffer.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildFramebuffer(const int32_t usedBuffer)
{
	VkImageView imageViews[2];

	imageViews[0] = swapchainImageView[usedBuffer]->getImageView();
	imageViews[1] = depthStencilImageView->getImageView();

	framebuffer[usedBuffer] = vkts::framebufferCreate(initialResources->getDevice()->getDevice(), 0, renderPass->getRenderPass(), 2, imageViews, swapchain->getImageExtent().width, swapchain->getImageExtent().height, 1);

	if (!framebuffer[usedBuffer].get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create frame buffer.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildSwapchainImageView(const int32_t usedBuffer)
{
	VkComponentMapping componentMapping = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	VkImageSubresourceRange imageSubresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

	swapchainImageView[usedBuffer] = vkts::imageViewCreate(initialResources->getDevice()->getDevice(), 0, swapchain->getAllSwapchainImages()[usedBuffer], VK_IMAGE_VIEW_TYPE_2D, swapchain->getImageFormat(), componentMapping, imageSubresourceRange);

	if (!swapchainImageView[usedBuffer].get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create color attachment view.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::updateDescriptorSets()
{
	memset(descriptorBufferInfos, 0, sizeof(descriptorBufferInfos));

	descriptorBufferInfos[0].buffer = vertexViewProjectionUniformBuffer->getBuffer()->getBuffer();
	descriptorBufferInfos[0].offset = 0;
	descriptorBufferInfos[0].range = vertexViewProjectionUniformBuffer->getBuffer()->getSize();

	descriptorBufferInfos[1].buffer = fragmentUniformBuffer->getBuffer()->getBuffer();
	descriptorBufferInfos[1].offset = 0;
	descriptorBufferInfos[1].range = fragmentUniformBuffer->getBuffer()->getSize();

	memset(writeDescriptorSets, 0, sizeof(writeDescriptorSets));

	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;

	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].dstSet = VK_NULL_HANDLE;	// Defined later.
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].dstBinding = VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION;
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].dstArrayElement = 0;
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].descriptorCount = 1;
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].pImageInfo = nullptr;
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].pBufferInfo = &descriptorBufferInfos[0];
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].pTexelBufferView = nullptr;

	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;

	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].dstSet = VK_NULL_HANDLE;	// Defined later.
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].dstBinding = VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT;
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].dstArrayElement = 0;
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].descriptorCount = 1;
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].pImageInfo = nullptr;
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].pBufferInfo = &descriptorBufferInfos[1];
	writeDescriptorSets[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].pTexelBufferView = nullptr;

	return VK_TRUE;
}

VkBool32 Example::buildScene(const vkts::ICommandBuffersSP& cmdBuffer)
{
	VkSamplerCreateInfo samplerCreateInfo;

	memset(&samplerCreateInfo, 0, sizeof(VkSamplerCreateInfo));

	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	samplerCreateInfo.flags = 0;
	samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerCreateInfo.mipLodBias = 0.0f;
	samplerCreateInfo.maxAnisotropy = 1.0f;
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = 0.0f;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

	VkImageViewCreateInfo imageViewCreateInfo;

	memset(&imageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));

	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = VK_NULL_HANDLE;		// Defined later.
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = VK_FORMAT_UNDEFINED;		// Defined later.
	imageViewCreateInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
	imageViewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

	sceneContext = vkts::scenegraphCreateContext(VK_FALSE, initialResources, cmdBuffer, samplerCreateInfo, imageViewCreateInfo, descriptorSetLayout);

	if (!sceneContext.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create cache.");

		return VK_FALSE;
	}

	//

	scene = vkts::scenegraphLoadScene(VKTS_SCENE_NAME, sceneContext);

	if (!scene.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not load scene.");

		return VK_FALSE;
	}

	vkts::logPrint(VKTS_LOG_INFO, "Example: Number objects: %d", scene->getNumberObjects());

	return VK_TRUE;
}

VkBool32 Example::buildDepthStencilImageView()
{
	VkComponentMapping componentMapping = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	VkImageSubresourceRange imageSubresourceRange = { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 };

	depthStencilImageView = vkts::imageViewCreate(initialResources->getDevice()->getDevice(), 0, depthTexture->getImage()->getImage(), VK_IMAGE_VIEW_TYPE_2D, depthTexture->getImage()->getFormat(), componentMapping, imageSubresourceRange);

	if (!depthStencilImageView.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create depth attachment view.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildDepthTexture(const vkts::ICommandBuffersSP& cmdBuffer)
{
	VkImageCreateInfo imageCreateInfo;

	memset(&imageCreateInfo, 0, sizeof(VkImageCreateInfo));

	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;

	imageCreateInfo.flags = 0;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = VK_FORMAT_D16_UNORM;
	imageCreateInfo.extent = {swapchain->getImageExtent().width, swapchain->getImageExtent().height, 1};
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.queueFamilyIndexCount = 0;
	imageCreateInfo.pQueueFamilyIndices = nullptr;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VkImageSubresourceRange subresourceRange = { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 };

	depthTexture = vkts::memoryImageCreate(initialResources, cmdBuffer, "DepthTexture", imageCreateInfo, 0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, subresourceRange, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (!depthTexture.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create depth texture.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildPipeline()
{
	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo[VKTS_SHADER_STAGE_COUNT];

	memset(&pipelineShaderStageCreateInfo, 0, sizeof(pipelineShaderStageCreateInfo));

	pipelineShaderStageCreateInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

	pipelineShaderStageCreateInfo[0].flags = 0;
	pipelineShaderStageCreateInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	pipelineShaderStageCreateInfo[0].module = vertexShaderModule->getShaderModule();
	pipelineShaderStageCreateInfo[0].pName = "main";
	pipelineShaderStageCreateInfo[0].pSpecializationInfo = nullptr;

	pipelineShaderStageCreateInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

	pipelineShaderStageCreateInfo[1].flags = 0;
	pipelineShaderStageCreateInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	pipelineShaderStageCreateInfo[1].module = fragmentShaderModule->getShaderModule();
	pipelineShaderStageCreateInfo[1].pName = "main";
	pipelineShaderStageCreateInfo[1].pSpecializationInfo = nullptr;

	//

	VkTsVertexBufferType vertexBufferType = VKTS_VERTEX_BUFFER_TYPE_VERTEX | VKTS_VERTEX_BUFFER_TYPE_TANGENTS | VKTS_VERTEX_BUFFER_TYPE_TEXCOORD;

	VkVertexInputBindingDescription vertexInputBindingDescription;

	memset(&vertexInputBindingDescription, 0, sizeof(VkVertexInputBindingDescription));

	vertexInputBindingDescription.binding = VKTS_BINDING_VERTEX_BUFFER;
	vertexInputBindingDescription.stride = vkts::commonGetStrideInBytes(vertexBufferType);
	vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription vertexInputAttributeDescription[3];

	memset(&vertexInputAttributeDescription, 0, sizeof(vertexInputAttributeDescription));

	vertexInputAttributeDescription[0].location = 0;
	vertexInputAttributeDescription[0].binding = VKTS_BINDING_VERTEX_BUFFER;
	vertexInputAttributeDescription[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
	vertexInputAttributeDescription[0].offset = vkts::commonGetOffsetInBytes(VKTS_VERTEX_BUFFER_TYPE_VERTEX, vertexBufferType);

	vertexInputAttributeDescription[1].location = 1;
	vertexInputAttributeDescription[1].binding = VKTS_BINDING_VERTEX_BUFFER;
	vertexInputAttributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexInputAttributeDescription[1].offset = vkts::commonGetOffsetInBytes(VKTS_VERTEX_BUFFER_TYPE_NORMAL, vertexBufferType);

	vertexInputAttributeDescription[2].location = 2;
	vertexInputAttributeDescription[2].binding = VKTS_BINDING_VERTEX_BUFFER;
	vertexInputAttributeDescription[2].format = VK_FORMAT_R32G32_SFLOAT;
	vertexInputAttributeDescription[2].offset = vkts::commonGetOffsetInBytes(VKTS_VERTEX_BUFFER_TYPE_TEXCOORD, vertexBufferType);

	VkPipelineVertexInputStateCreateInfo pipelineVertexInputCreateInfo;

	memset(&pipelineVertexInputCreateInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));

	pipelineVertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	pipelineVertexInputCreateInfo.flags = 0;
	pipelineVertexInputCreateInfo.vertexBindingDescriptionCount = 1;
	pipelineVertexInputCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
	pipelineVertexInputCreateInfo.vertexAttributeDescriptionCount = 3;
	pipelineVertexInputCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescription;

	//

	VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;

	memset(&pipelineInputAssemblyStateCreateInfo, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));

	pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

	pipelineInputAssemblyStateCreateInfo.flags = 0;
	pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	//

	VkViewport viewport;
	memset(&viewport, 0, sizeof(VkViewport));

	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapchain->getImageExtent().width;
	viewport.height = (float) swapchain->getImageExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor;
	memset(&scissor, 0, sizeof(VkRect2D));

	scissor.offset.x = 0;
	scissor.offset.y = 0;
	scissor.extent = swapchain->getImageExtent();

	VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;

	memset(&pipelineViewportStateCreateInfo, 0, sizeof(VkPipelineViewportStateCreateInfo));

	pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

	pipelineViewportStateCreateInfo.flags = 0;
	pipelineViewportStateCreateInfo.viewportCount = 1;
	pipelineViewportStateCreateInfo.pViewports = &viewport;
	pipelineViewportStateCreateInfo.scissorCount = 1;
	pipelineViewportStateCreateInfo.pScissors = &scissor;

	//

	VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo;

	memset(&pipelineRasterizationStateCreateInfo, 0, sizeof(VkPipelineRasterizationStateCreateInfo));

	pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

	pipelineRasterizationStateCreateInfo.flags = 0;
	pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
	pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
	pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
	pipelineRasterizationStateCreateInfo.lineWidth = 0.0f;

	//

	VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;

	memset(&pipelineMultisampleStateCreateInfo, 0, sizeof(VkPipelineMultisampleStateCreateInfo));

	pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

	pipelineMultisampleStateCreateInfo.flags = 0;
	pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	pipelineMultisampleStateCreateInfo.minSampleShading = 0.0f;
	pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
	pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
	pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;

	//

	VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo;

	memset(&pipelineDepthStencilStateCreateInfo, 0, sizeof(VkPipelineDepthStencilStateCreateInfo));

	pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

	pipelineDepthStencilStateCreateInfo.flags = 0;
	pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
	pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
	pipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
	pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
	pipelineDepthStencilStateCreateInfo.front.failOp = VK_STENCIL_OP_KEEP;
	pipelineDepthStencilStateCreateInfo.front.passOp = VK_STENCIL_OP_KEEP;
	pipelineDepthStencilStateCreateInfo.front.depthFailOp = VK_STENCIL_OP_KEEP;
	pipelineDepthStencilStateCreateInfo.front.compareOp = VK_COMPARE_OP_NEVER;
	pipelineDepthStencilStateCreateInfo.front.compareMask = 0;
	pipelineDepthStencilStateCreateInfo.front.writeMask = 0;
	pipelineDepthStencilStateCreateInfo.front.reference = 0;
	pipelineDepthStencilStateCreateInfo.back.failOp = VK_STENCIL_OP_KEEP;
	pipelineDepthStencilStateCreateInfo.back.passOp = VK_STENCIL_OP_KEEP;
	pipelineDepthStencilStateCreateInfo.back.depthFailOp = VK_STENCIL_OP_KEEP;
	pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_NEVER;
	pipelineDepthStencilStateCreateInfo.back.compareMask = 0;
	pipelineDepthStencilStateCreateInfo.back.writeMask = 0;
	pipelineDepthStencilStateCreateInfo.back.reference = 0;
	pipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;

	//

	VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;

	memset(&pipelineColorBlendAttachmentState, 0, sizeof(VkPipelineColorBlendAttachmentState));

	pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;
	pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	pipelineColorBlendAttachmentState.colorWriteMask = 0xf;

	VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo;

	memset(&pipelineColorBlendStateCreateInfo, 0, sizeof(VkPipelineColorBlendStateCreateInfo));

	pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

	pipelineColorBlendStateCreateInfo.flags = 0;
	pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
	pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	pipelineColorBlendStateCreateInfo.attachmentCount = 1;
	pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
	pipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f;
	pipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f;
	pipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f;
	pipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f;

	//

	VkDynamicState dynamicState[VKTS_NUMBER_DYNAMIC_STATES] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

	VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo;

	memset(&pipelineDynamicStateCreateInfo, 0, sizeof(VkPipelineDynamicStateCreateInfo));

	pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

	pipelineDynamicStateCreateInfo.flags = 0;
	pipelineDynamicStateCreateInfo.dynamicStateCount = VKTS_NUMBER_DYNAMIC_STATES;
	pipelineDynamicStateCreateInfo.pDynamicStates = dynamicState;

	//

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;

	memset(&graphicsPipelineCreateInfo, 0, sizeof(VkGraphicsPipelineCreateInfo));

	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	graphicsPipelineCreateInfo.flags = 0;
	graphicsPipelineCreateInfo.stageCount = VKTS_SHADER_STAGE_COUNT;
	graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfo;
	graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.pTessellationState = nullptr;
	graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = &pipelineDepthStencilStateCreateInfo;
	graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
	graphicsPipelineCreateInfo.layout = pipelineLayout->getPipelineLayout();
	graphicsPipelineCreateInfo.renderPass = renderPass->getRenderPass();
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineCreateInfo.basePipelineIndex = 0;

	auto pipeline = vkts::pipelineCreateGraphics(initialResources->getDevice()->getDevice(), VK_NULL_HANDLE, graphicsPipelineCreateInfo, vertexBufferType);

	if (!pipeline.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create graphics pipeline.");

		return VK_FALSE;
	}

	allGraphicsPipelines.append(pipeline);

	return VK_TRUE;
}

VkBool32 Example::buildRenderPass()
{
	VkAttachmentDescription attachmentDescription[2];

	memset(&attachmentDescription, 0, sizeof(attachmentDescription));

	attachmentDescription[0].flags = 0;
	attachmentDescription[0].format = swapchain->getImageFormat();
	attachmentDescription[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachmentDescription[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	attachmentDescription[1].flags = 0;
	attachmentDescription[1].format = VK_FORMAT_D16_UNORM;
	attachmentDescription[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachmentDescription[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentReference;

	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference deptStencilAttachmentReference;

	deptStencilAttachmentReference.attachment = 1;
	deptStencilAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription;

	memset(&subpassDescription, 0, sizeof(VkSubpassDescription));

	subpassDescription.flags = 0;
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = nullptr;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorAttachmentReference;
	subpassDescription.pResolveAttachments = nullptr;
	subpassDescription.pDepthStencilAttachment = &deptStencilAttachmentReference;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = nullptr;

	renderPass = vkts::renderPassCreate( initialResources->getDevice()->getDevice(), 0, 2, attachmentDescription, 1, &subpassDescription, 0, nullptr);

	if (!renderPass.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create render pass.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildPipelineLayout()
{
	VkDescriptorSetLayout setLayouts[1];

	setLayouts[0] = descriptorSetLayout->getDescriptorSetLayout();

	pipelineLayout = vkts::pipelineCreateLayout(initialResources->getDevice()->getDevice(), 0, 1, setLayouts, 0, nullptr);

	if (!pipelineLayout.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create pipeline layout.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM];

	memset(&descriptorSetLayoutBinding, 0, sizeof(descriptorSetLayoutBinding));

	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].binding = VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].descriptorCount = 1;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION].pImmutableSamplers = nullptr;

	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM].binding = VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM].descriptorCount = 1;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM].pImmutableSamplers = nullptr;

	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].binding = VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].descriptorCount = 1;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	descriptorSetLayoutBinding[VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT].pImmutableSamplers = nullptr;

    for (int32_t i = VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE; i < VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM; i++)
    {
		descriptorSetLayoutBinding[i].binding = i;
		descriptorSetLayoutBinding[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorSetLayoutBinding[i].descriptorCount = 1;
		descriptorSetLayoutBinding[i].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		descriptorSetLayoutBinding[i].pImmutableSamplers = nullptr;
    }

    //

    descriptorSetLayout = vkts::descriptorSetLayoutCreate(initialResources->getDevice()->getDevice(), 0, VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM, descriptorSetLayoutBinding);

	if (!descriptorSetLayout.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create descriptor set layout.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildShader()
{
	auto vertexShaderBinary = vkts::fileLoadBinary(VKTS_VERTEX_SHADER_NAME);

	if (!vertexShaderBinary.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not load vertex shader: '%s'", VKTS_VERTEX_SHADER_NAME);

		return VK_FALSE;
	}

	auto fragmentShaderBinary = vkts::fileLoadBinary(VKTS_FRAGMENT_SHADER_NAME);

	if (!fragmentShaderBinary.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not load fragment shader: '%s'", VKTS_FRAGMENT_SHADER_NAME);

		return VK_FALSE;
	}

	//

	vertexShaderModule = vkts::shaderModuleCreate(initialResources->getDevice()->getDevice(), 0, vertexShaderBinary->getSize(), (uint32_t*)vertexShaderBinary->getData());

	if (!vertexShaderModule.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create vertex shader module.");

		return VK_FALSE;
	}

	fragmentShaderModule = vkts::shaderModuleCreate(initialResources->getDevice()->getDevice(), 0, fragmentShaderBinary->getSize(), (uint32_t*)fragmentShaderBinary->getData());

	if (!fragmentShaderModule.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create fragment shader module.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildUniformBuffers()
{
	VkBufferCreateInfo bufferCreateInfo;

	memset(&bufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = vkts::commonGetDeviceSize(16 * sizeof(float) * 2, 16);
	bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = nullptr;

	vertexViewProjectionUniformBuffer = vkts::bufferObjectCreate(initialResources, bufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

	if (!vertexViewProjectionUniformBuffer.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create vertex uniform buffer.");

		return VK_FALSE;
	}

	memset(&bufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = vkts::commonGetDeviceSize(3 * sizeof(float), 16);
	bufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = nullptr;

	fragmentUniformBuffer = vkts::bufferObjectCreate(initialResources, bufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

	if (!fragmentUniformBuffer.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create fragment uniform buffer.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildResources(const vkts::IUpdateThreadContext& updateContext)
{
	VkResult result;

	//

	auto lastSwapchain = swapchain;

	VkSwapchainKHR oldSwapchain = lastSwapchain.get() ? lastSwapchain->getSwapchain() : VK_NULL_HANDLE;

	swapchain = vkts::wsiSwapchainCreate(initialResources->getPhysicalDevice()->getPhysicalDevice(), initialResources->getDevice()->getDevice(), 0, surface->getSurface(), VKTS_NUMBER_BUFFERS, 1, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_SHARING_MODE_EXCLUSIVE, 0, nullptr, VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, VK_TRUE, oldSwapchain);

	if (!swapchain.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create swap chain.");

		return VK_FALSE;
	}

    //

    swapchainImagesCount = (uint32_t)swapchain->getAllSwapchainImages().size();

    if (swapchainImagesCount == 0)
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get swap chain images count.");

        return VK_FALSE;
    }

    swapchainImageView = vkts::SmartPointerVector<vkts::IImageViewSP>(swapchainImagesCount);
    framebuffer = vkts::SmartPointerVector<vkts::IFramebufferSP>(swapchainImagesCount);
    cmdBuffer = vkts::SmartPointerVector<vkts::ICommandBuffersSP>(swapchainImagesCount);

    //

	if (lastSwapchain.get())
	{
		lastSwapchain->destroy();
	}

	//

	if (!buildRenderPass())
	{
		return VK_FALSE;
	}

	if (!buildPipeline())
	{
		return VK_FALSE;
	}

	//

	vkts::ICommandBuffersSP updateCmdBuffer = vkts::commandBuffersCreate(initialResources->getDevice()->getDevice(), commandPool->getCmdPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

	if (!updateCmdBuffer.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create command buffer.");

		return VK_FALSE;
	}


	result = updateCmdBuffer->beginCommandBuffer(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, VK_FALSE, 0, 0);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not begin command buffer.");

		return VK_FALSE;
	}

    for (uint32_t index = 0; index < swapchain->getMinImageCount(); index++)
    {
        swapchain->cmdPipelineBarrier(updateCmdBuffer->getCommandBuffer(), 0, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, index);
    }

	if (!buildDepthTexture(updateCmdBuffer))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build texture.");

		return VK_FALSE;
	}

	VkBool32 doUpdateDescriptorSets = VK_FALSE;

	if (!scene.get())
	{
		if (!buildScene(updateCmdBuffer))
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build scene.");

			return VK_FALSE;
		}

		doUpdateDescriptorSets = VK_TRUE;
	}

	result = updateCmdBuffer->endCommandBuffer();

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not end command buffer.");

		return VK_FALSE;
	}


	VkSubmitInfo submitInfo;

	memset(&submitInfo, 0, sizeof(VkSubmitInfo));

	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.commandBufferCount = updateCmdBuffer->getCommandBufferCount();
	submitInfo.pCommandBuffers = updateCmdBuffer->getCommandBuffers();
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;

	result = initialResources->getQueue()->submit(1, &submitInfo, VK_NULL_HANDLE);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not submit queue.");

		return VK_FALSE;
	}

	result = initialResources->getQueue()->waitIdle();

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not wait for idle queue.");

		return VK_FALSE;
	}

	updateCmdBuffer->destroy();

	//

	if (doUpdateDescriptorSets)
	{
		if (!updateDescriptorSets())
		{
			return VK_FALSE;
		}

		if (scene.get())
		{
			scene->updateDescriptorSetsRecursive(VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM, writeDescriptorSets);
		}
	}

	if (!buildDepthStencilImageView())
	{
		return VK_FALSE;
	}

	//

	for (int32_t i = 0; i < (int32_t)swapchainImagesCount; i++)
	{
		if (!buildSwapchainImageView(i))
		{
			return VK_FALSE;
		}

		if (!buildFramebuffer(i))
		{
			return VK_FALSE;
		}

		if (!buildCmdBuffer(i))
		{
			return VK_FALSE;
		}
	}

	return VK_TRUE;
}

void Example::terminateResources(const vkts::IUpdateThreadContext& updateContext)
{
	if (initialResources.get())
	{
		if (initialResources->getDevice().get())
		{
			for (int32_t i = 0; i < (int32_t)swapchainImagesCount; i++)
			{
				if (cmdBuffer[i].get())
				{
					cmdBuffer[i]->destroy();
				}

				if (framebuffer[i].get())
				{
					framebuffer[i]->destroy();
				}

				if (swapchainImageView[i].get())
				{
					swapchainImageView[i]->destroy();
				}
			}

			if (depthStencilImageView.get())
			{
				depthStencilImageView->destroy();
			}

			if (depthTexture.get())
			{
				depthTexture->destroy();
			}

			for (size_t i = 0; i < allGraphicsPipelines.size(); i++)
			{
				allGraphicsPipelines[i]->destroy();
			}
			allGraphicsPipelines.clear();

			if (renderPass.get())
			{
				renderPass->destroy();
			}
		}
	}
}

//
// Vulkan initialization.
//
VkBool32 Example::init(const vkts::IUpdateThreadContext& updateContext)
{
	if (!updateContext.isWindowAttached(windowIndex))
	{
		return VK_FALSE;
	}

	windowDimension = updateContext.getWindowDimension(windowIndex);

	//

	commandPool = vkts::commandPoolCreate(initialResources->getDevice()->getDevice(), 0, initialResources->getQueue()->getQueueFamilyIndex());

	if (!commandPool.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get command pool.");

		return VK_FALSE;
	}

	//

    imageAcquiredSemaphore = vkts::semaphoreCreate(initialResources->getDevice()->getDevice(), 0);

    if (!imageAcquiredSemaphore.get())
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create semaphore.");

        return VK_FALSE;
    }

    renderingCompleteSemaphore = vkts::semaphoreCreate(initialResources->getDevice()->getDevice(), 0);

    if (!renderingCompleteSemaphore.get())
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create semaphore.");

        return VK_FALSE;
    }

	//

	if (!buildUniformBuffers())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build uniform buffers.");

		return VK_FALSE;
	}

	if (!buildShader())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build shader.");

		return VK_FALSE;
	}

	if (!buildDescriptorSetLayout())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build descriptor set layout.");

		return VK_FALSE;
	}

	if (!buildPipelineLayout())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build pipeline cache.");

		return VK_FALSE;
	}

	//

	if (!buildResources(updateContext))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build resources.");

		return VK_FALSE;
	}

	//

	glm::vec3 lightDirection = glm::vec3(0.0f, 2.0f, 1.0f);

	lightDirection = glm::normalize(lightDirection);

	if (!fragmentUniformBuffer->upload(0, 0, lightDirection))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not upload light direction.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

//
// Vulkan update.
//
VkBool32 Example::update(const vkts::IUpdateThreadContext& updateContext)
{

	VkResult result = VK_SUCCESS;

	//

	if (windowDimension != updateContext.getWindowDimension(windowIndex))
	{
		windowDimension = updateContext.getWindowDimension(windowIndex);

		result = VK_ERROR_OUT_OF_DATE_KHR;
	}

	//

	uint32_t currentBuffer;

	if (result == VK_SUCCESS)
	{
		result = swapchain->acquireNextImage(UINT64_MAX, imageAcquiredSemaphore->getSemaphore(), VK_NULL_HANDLE, currentBuffer);
	}

	if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
	{
		glm::mat4 projectionMatrix(1.0f);
		glm::mat4 viewMatrix(1.0f);

		const auto& dimension = updateContext.getWindowDimension(windowIndex);

		projectionMatrix = vkts::perspectiveMat4(45.0f, (float) dimension.x / (float) dimension.y, 1.0f, 100.0f);
		viewMatrix = vkts::lookAtMat4(0.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		if (!vertexViewProjectionUniformBuffer->upload(0 * sizeof(float) * 16, 0, projectionMatrix))
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not upload matrices.");

			return VK_FALSE;
		}
		if (!vertexViewProjectionUniformBuffer->upload(1 * sizeof(float) * 16, 0, viewMatrix))
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not upload matrices.");

			return VK_FALSE;
		}

		if (scene.get())
		{
			scene->updateRecursive(updateContext);
		}

		//

        VkSemaphore waitSemaphores = imageAcquiredSemaphore->getSemaphore();
        VkSemaphore signalSemaphores = renderingCompleteSemaphore->getSemaphore();


        VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

        VkSubmitInfo submitInfo;

        memset(&submitInfo, 0, sizeof(VkSubmitInfo));

        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &waitSemaphores;
        submitInfo.pWaitDstStageMask = &waitDstStageMask;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = cmdBuffer[currentBuffer]->getCommandBuffers();
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &signalSemaphores;

		result = initialResources->getQueue()->submit(1, &submitInfo, VK_NULL_HANDLE);

		if (result != VK_SUCCESS)
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not submit queue.");

			return VK_FALSE;
		}

        waitSemaphores = renderingCompleteSemaphore->getSemaphore();

        VkSwapchainKHR swapchains = swapchain->getSwapchain();

        result = swapchain->queuePresent(initialResources->getQueue()->getQueue(), 1, &waitSemaphores, 1, &swapchains, &currentBuffer, nullptr);

		if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
		{
			result = initialResources->getQueue()->waitIdle();

			if (result != VK_SUCCESS)
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not wait for idle queue.");

				return VK_FALSE;
			}
		}
		else
		{
			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				terminateResources(updateContext);

				if (!buildResources(updateContext))
				{
					vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build resources.");

					return VK_FALSE;
				}
			}
			else
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not present queue.");

				return VK_FALSE;
			}
		}
	}
	else
	{
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			terminateResources(updateContext);

			if (!buildResources(updateContext))
			{
				vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build resources.");

				return VK_FALSE;
			}
		}
		else
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not acquire next image.");

			return VK_FALSE;
		}
	}

	//

    result = imageAcquiredSemaphore->reset();

    if (result != VK_SUCCESS)
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not reset semaphore.");

        return VK_FALSE;
    }

    result = renderingCompleteSemaphore->reset();

    if (result != VK_SUCCESS)
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not reset semaphore.");

        return VK_FALSE;
    }

	return VK_TRUE;
}

//
// Vulkan termination.
//
void Example::terminate(const vkts::IUpdateThreadContext& updateContext)
{
	if (initialResources.get())
	{
		if (initialResources->getDevice().get())
		{
			terminateResources(updateContext);

			//

			if (sceneContext.get())
			{
				sceneContext->destroy();

				sceneContext.reset();
			}

			if (scene.get())
			{
				scene->destroy();
			}

			if (swapchain.get())
			{
				swapchain->destroy();
			}

			if (pipelineLayout.get())
			{
				pipelineLayout->destroy();
			}

			if (vertexShaderModule.get())
			{
				vertexShaderModule->destroy();
			}

			if (fragmentShaderModule.get())
			{
				fragmentShaderModule->destroy();
			}

			if (vertexViewProjectionUniformBuffer.get())
			{
				vertexViewProjectionUniformBuffer->destroy();
			}

			if (fragmentUniformBuffer.get())
			{
				fragmentUniformBuffer->destroy();
			}

			if (descriptorSetLayout.get())
			{
				descriptorSetLayout->destroy();
			}

	        if (renderingCompleteSemaphore.get())
	        {
	            renderingCompleteSemaphore->destroy();
	        }

	        if (imageAcquiredSemaphore.get())
	        {
	            imageAcquiredSemaphore->destroy();
	        }

			if (commandPool.get())
			{
				commandPool->destroy();
			}
		}
	}
}
