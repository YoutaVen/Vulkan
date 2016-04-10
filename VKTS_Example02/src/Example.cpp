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

Example::Example(const vkts::IInstanceSP& instance, const vkts::IPhysicalDeviceSP& physicalDevice, const int32_t windowIndex, const vkts::ISurfaceSP& surface, const vkts::IDeviceSP& device, const vkts::IQueueSP& queue) :
		IUpdateThread(), instance(instance), physicalDevice(physicalDevice), windowIndex(windowIndex), surface(surface), device(device), queue(queue), windowDimension(0, 0), commandPool(nullptr), imageAcquiredSemaphore(nullptr), renderingCompleteSemaphore(nullptr), vertexBuffer(VK_NULL_HANDLE), deviceMemoryVertexBuffer(VK_NULL_HANDLE), vertexShaderModule(VK_NULL_HANDLE), fragmentShaderModule(VK_NULL_HANDLE), pipelineLayout(VK_NULL_HANDLE), swapchain(nullptr), renderPass(nullptr), pipeline(VK_NULL_HANDLE), swapchainImagesCount(0), swapchainImageView(), framebuffer(), cmdBuffer()
{
}

Example::~Example()
{
}

VkBool32 Example::buildCmdBuffer(const int32_t usedBuffer)
{
	VkResult result;

	cmdBuffer[usedBuffer] = vkts::commandBuffersCreate(device->getDevice(), commandPool->getCmdPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

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

	clearColorValue.float32[0] = 0.0f;
	clearColorValue.float32[1] = 0.0f;
	clearColorValue.float32[2] = 1.0f;
	clearColorValue.float32[3] = 1.0f;

	VkClearValue clearValues[1] = {clearColorValue};

	VkRenderPassBeginInfo renderPassBeginInfo;

	memset(&renderPassBeginInfo, 0, sizeof(VkRenderPassBeginInfo));

	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = renderPass->getRenderPass();
	renderPassBeginInfo.framebuffer = framebuffer[usedBuffer]->getFramebuffer();
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent = swapchain->getImageExtent();
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = clearValues;

	cmdBuffer[usedBuffer]->cmdBeginRenderPass(&renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(cmdBuffer[usedBuffer]->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);


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

	VkDeviceSize offsets[1] = {0};

	vkCmdBindVertexBuffers(cmdBuffer[usedBuffer]->getCommandBuffer(), 0, 1, &vertexBuffer, offsets);

	vkCmdDraw(cmdBuffer[usedBuffer]->getCommandBuffer(), 3, 1, 0, 0);

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
	auto imageView = swapchainImageView[usedBuffer]->getImageView();

	framebuffer[usedBuffer] = vkts::framebufferCreate(device->getDevice(), 0, renderPass->getRenderPass(), 1, &imageView, swapchain->getImageExtent().width, swapchain->getImageExtent().height, 1);

	if (!framebuffer[usedBuffer].get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create frame buffer.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildSwapchainImageView(const int32_t usedBuffer)
{
	VkComponentMapping componentMapping = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};

	VkImageSubresourceRange imageSubresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

	swapchainImageView[usedBuffer] = vkts::imageViewCreate(device->getDevice(), 0, swapchain->getAllSwapchainImages()[usedBuffer], VK_IMAGE_VIEW_TYPE_2D, swapchain->getImageFormat(), componentMapping, imageSubresourceRange);

	if (!swapchainImageView[usedBuffer].get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create color attachment view.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildPipeline()
{
	VkResult result;

	//

	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo[VKTS_SHADER_STAGE_COUNT];

	memset(&pipelineShaderStageCreateInfo, 0, sizeof(pipelineShaderStageCreateInfo));

	pipelineShaderStageCreateInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

	pipelineShaderStageCreateInfo[0].flags = 0;
	pipelineShaderStageCreateInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	pipelineShaderStageCreateInfo[0].module = vertexShaderModule;
	pipelineShaderStageCreateInfo[0].pName = "main";
	pipelineShaderStageCreateInfo[0].pSpecializationInfo = nullptr;


	pipelineShaderStageCreateInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

	pipelineShaderStageCreateInfo[1].flags = 0;
	pipelineShaderStageCreateInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	pipelineShaderStageCreateInfo[1].module = fragmentShaderModule;
	pipelineShaderStageCreateInfo[1].pName = "main";
	pipelineShaderStageCreateInfo[1].pSpecializationInfo = nullptr;

	//

	VkVertexInputBindingDescription vertexInputBindingDescription;

	memset(&vertexInputBindingDescription, 0, sizeof(VkVertexInputBindingDescription));

	vertexInputBindingDescription.binding = 0;
	vertexInputBindingDescription.stride = 4 * sizeof(float);
	vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	VkVertexInputAttributeDescription vertexInputAttributeDescription;

	memset(&vertexInputAttributeDescription, 0, sizeof(VkVertexInputAttributeDescription));

	vertexInputAttributeDescription.location = 0;
	vertexInputAttributeDescription.binding = 0;
	vertexInputAttributeDescription.format = VK_FORMAT_R32G32B32A32_SFLOAT;
	vertexInputAttributeDescription.offset = 0;

	VkPipelineVertexInputStateCreateInfo pipelineVertexInputCreateInfo;

	memset(&pipelineVertexInputCreateInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));

	pipelineVertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	pipelineVertexInputCreateInfo.flags = 0;
	pipelineVertexInputCreateInfo.vertexBindingDescriptionCount = 1;
	pipelineVertexInputCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;
	pipelineVertexInputCreateInfo.vertexAttributeDescriptionCount = 1;
	pipelineVertexInputCreateInfo.pVertexAttributeDescriptions = &vertexInputAttributeDescription;

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
	pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
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

	VkDynamicState dynamicState[VKTS_NUMBER_DYNAMIC_STATES] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

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
	graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
	graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
	graphicsPipelineCreateInfo.layout = pipelineLayout;
	graphicsPipelineCreateInfo.renderPass = renderPass->getRenderPass();
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	graphicsPipelineCreateInfo.basePipelineIndex = 0;

	result = vkCreateGraphicsPipelines(device->getDevice(), VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &pipeline);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create graphics pipeline.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildRenderPass()
{
	VkAttachmentDescription attachmentDescription;

	memset(&attachmentDescription, 0, sizeof(VkAttachmentDescription));

	attachmentDescription.flags = 0;
	attachmentDescription.format = swapchain->getImageFormat();
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentReference;

	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription;

	memset(&subpassDescription, 0, sizeof(VkSubpassDescription));

	subpassDescription.flags = 0;
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pInputAttachments = nullptr;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorAttachmentReference;
	subpassDescription.pResolveAttachments = nullptr;
	subpassDescription.pDepthStencilAttachment = nullptr;
	subpassDescription.preserveAttachmentCount = 0;
	subpassDescription.pPreserveAttachments = nullptr;

	renderPass = vkts::renderPassCreate(device->getDevice(), 0, 1, &attachmentDescription, 1, &subpassDescription, 0, nullptr);

	if (!renderPass.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create render pass.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildPipelineLayout()
{
	VkResult result;

	//

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;

	memset(&pipelineLayoutCreateInfo, 0, sizeof(VkPipelineLayoutCreateInfo));

	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	pipelineLayoutCreateInfo.flags = 0;
	pipelineLayoutCreateInfo.setLayoutCount = 0;
	pipelineLayoutCreateInfo.pSetLayouts = nullptr;
	pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
	pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

	result = vkCreatePipelineLayout(device->getDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create pipeline layout.");

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

	VkResult result;

	VkShaderModuleCreateInfo shaderModuleCreateInfo;

	memset(&shaderModuleCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));

	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	shaderModuleCreateInfo.flags = 0;
	shaderModuleCreateInfo.codeSize = vertexShaderBinary->getSize();
	shaderModuleCreateInfo.pCode = (const uint32_t*)vertexShaderBinary->getData();

	result = vkCreateShaderModule(device->getDevice(), &shaderModuleCreateInfo, nullptr, &vertexShaderModule);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create vertex shader module.");

		return VK_FALSE;
	}


	memset(&shaderModuleCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));

	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	shaderModuleCreateInfo.flags = 0;
	shaderModuleCreateInfo.codeSize = fragmentShaderBinary->getSize();
	shaderModuleCreateInfo.pCode = (const uint32_t*)fragmentShaderBinary->getData();

	result = vkCreateShaderModule(device->getDevice(), &shaderModuleCreateInfo, nullptr, &fragmentShaderModule);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create fragment shader module.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildVertexBuffer()
{
	// Window clip origin is upper left.
	static const float vertices[3 * 4] = {-0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.0f, 1.0f};

	VkResult result;

	VkBufferCreateInfo bufferCreateInfo;

	memset(&bufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

	bufferCreateInfo.flags = 0;
	bufferCreateInfo.size = sizeof(vertices);
	bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.queueFamilyIndexCount = 0;
	bufferCreateInfo.pQueueFamilyIndices = nullptr;

	result = vkCreateBuffer(device->getDevice(), &bufferCreateInfo, nullptr, &vertexBuffer);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create vertex buffer.");

		return VK_FALSE;
	}

	VkMemoryRequirements memoryRequirements;

	vkGetBufferMemoryRequirements(device->getDevice(), vertexBuffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocInfo;

	memset(&memoryAllocInfo, 0, sizeof(VkMemoryAllocateInfo));

	memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

	memoryAllocInfo.allocationSize = memoryRequirements.size;
	memoryAllocInfo.memoryTypeIndex = 0;	// Gathered in next function.

	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
	physicalDevice->getPhysicalDeviceMemoryProperties(physicalDeviceMemoryProperties);

	if (!vkts::commonGetMemoryTypeIndex(VK_MAX_MEMORY_TYPES, physicalDeviceMemoryProperties.memoryTypes, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, memoryAllocInfo.memoryTypeIndex))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get memory type index.");

		return VK_FALSE;
	}

	void* mappedData;

	result = vkAllocateMemory(device->getDevice(), &memoryAllocInfo, nullptr, &deviceMemoryVertexBuffer);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not allocate memory.");

		return VK_FALSE;
	}

	result = vkMapMemory(device->getDevice(), deviceMemoryVertexBuffer, 0, memoryRequirements.size, 0, &mappedData);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not map memory.");

		return VK_FALSE;
	}

	memcpy(mappedData, vertices, sizeof(vertices));

	vkUnmapMemory(device->getDevice(), deviceMemoryVertexBuffer);

	result = vkBindBufferMemory(device->getDevice(), vertexBuffer, deviceMemoryVertexBuffer, 0);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not bind buffer memory.");

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

	swapchain = vkts::wsiSwapchainCreate(physicalDevice->getPhysicalDevice(), device->getDevice(), 0, surface->getSurface(), VKTS_NUMBER_BUFFERS, 1, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_SHARING_MODE_EXCLUSIVE, 0, nullptr, VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, VK_TRUE, oldSwapchain);

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

	vkts::ICommandBuffersSP updateCmdBuffer = vkts::commandBuffersCreate(device->getDevice(), commandPool->getCmdPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);

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

	result = queue->submit(1, &submitInfo, VK_NULL_HANDLE);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not submit queue.");

		return VK_FALSE;
	}

	result = queue->waitIdle();

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not wait for idle queue.");

		return VK_FALSE;
	}

	updateCmdBuffer->destroy();

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
	if (device.get())
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

		if (pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(device->getDevice(), pipeline, nullptr);

			pipeline = VK_NULL_HANDLE;
		}

		if (renderPass.get())
		{
			renderPass->destroy();
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

	commandPool = vkts::commandPoolCreate(device->getDevice(), 0, queue->getQueueFamilyIndex());

	if (!commandPool.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get command pool.");

		return VK_FALSE;
	}

	//

	imageAcquiredSemaphore = vkts::semaphoreCreate(device->getDevice(), 0);

	if (!imageAcquiredSemaphore.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create semaphore.");

		return VK_FALSE;
	}

    renderingCompleteSemaphore = vkts::semaphoreCreate(device->getDevice(), 0);

    if (!renderingCompleteSemaphore.get())
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create semaphore.");

        return VK_FALSE;
    }

	//

	if (!buildVertexBuffer())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build vertex buffer.");

		return VK_FALSE;
	}

	if (!buildShader())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build shader.");

		return VK_FALSE;
	}

	if (!buildPipelineLayout())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build pipeline layout.");

		return VK_FALSE;
	}

	//

	if (!buildResources(updateContext))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build resources.");

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

		result = queue->submit(1, &submitInfo, VK_NULL_HANDLE);

		if (result != VK_SUCCESS)
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not submit queue.");

			return VK_FALSE;
		}

		waitSemaphores = renderingCompleteSemaphore->getSemaphore();

		VkSwapchainKHR swapchains = swapchain->getSwapchain();

		result = swapchain->queuePresent(queue->getQueue(), 1, &waitSemaphores, 1, &swapchains, &currentBuffer, nullptr);

		if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
		{
			result = queue->waitIdle();

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
	if (device.get())
	{
		terminateResources(updateContext);

		//

		if (swapchain.get())
		{
			swapchain->destroy();
		}

		if (pipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(device->getDevice(), pipelineLayout, nullptr);

			pipelineLayout = VK_NULL_HANDLE;
		}

		if (vertexShaderModule != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(device->getDevice(), vertexShaderModule, nullptr);

			vertexShaderModule = VK_NULL_HANDLE;
		}

		if (fragmentShaderModule != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(device->getDevice(), fragmentShaderModule, nullptr);

			fragmentShaderModule = VK_NULL_HANDLE;
		}

		if (vertexBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device->getDevice(), vertexBuffer, nullptr);

			vertexBuffer = VK_NULL_HANDLE;
		}

		if (deviceMemoryVertexBuffer != VK_NULL_HANDLE)
		{
			vkFreeMemory(device->getDevice(), deviceMemoryVertexBuffer, nullptr);

			deviceMemoryVertexBuffer = VK_NULL_HANDLE;
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
