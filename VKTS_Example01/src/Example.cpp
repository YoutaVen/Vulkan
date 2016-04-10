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

Example::Example(const VkInstance instance, const VkPhysicalDevice physicalDevice, const int32_t windowIndex, const vkts::ISurfaceSP& surface, const VkDevice device, const uint32_t queueFamilyIndex, const VkQueue queue) :
		IUpdateThread(), instance(instance), physicalDevice(physicalDevice), windowIndex(windowIndex), surface(surface), device(device), queueFamilyIndex(queueFamilyIndex), queue(queue), windowDimension(), commandPool(VK_NULL_HANDLE), imageAcquiredSemaphore(VK_NULL_HANDLE), renderingCompleteSemaphore(VK_NULL_HANDLE), swapchain(VK_NULL_HANDLE), renderPass(VK_NULL_HANDLE), swapchainImagesCount(0), swapchainImage(), swapchainImageView(), framebuffer(), cmdBuffer()
{
	memset(&swapchainCreateInfo, 0, sizeof(VkSwapchainCreateInfoKHR));
}

Example::~Example()
{
}

VkBool32 Example::buildCmdBuffer(const int32_t usedBuffer)
{
	VkResult result;

	//

	VkCommandBufferAllocateInfo cmdBufferCreateInfo;

	memset(&cmdBufferCreateInfo, 0, sizeof(VkCommandBufferAllocateInfo));

	cmdBufferCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

	cmdBufferCreateInfo.commandPool = commandPool;
	cmdBufferCreateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferCreateInfo.commandBufferCount = 1;

	result = vkAllocateCommandBuffers(device, &cmdBufferCreateInfo, &cmdBuffer[usedBuffer]);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create command buffer.");

		return VK_FALSE;
	}


	VkCommandBufferInheritanceInfo commandBufferInheritanceInfo;

	memset(&commandBufferInheritanceInfo, 0, sizeof(VkCommandBufferInheritanceInfo));

	commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;

	commandBufferInheritanceInfo.renderPass = VK_NULL_HANDLE;
	commandBufferInheritanceInfo.subpass = 0;
	commandBufferInheritanceInfo.framebuffer = VK_NULL_HANDLE;
	commandBufferInheritanceInfo.occlusionQueryEnable = VK_FALSE;
	commandBufferInheritanceInfo.queryFlags = 0;
	commandBufferInheritanceInfo.pipelineStatistics = 0;

	VkCommandBufferBeginInfo commandBufferBeginInfo;

	memset(&commandBufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));

	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	commandBufferBeginInfo.flags = 0;
	commandBufferBeginInfo.pInheritanceInfo = &commandBufferInheritanceInfo;

	result = vkBeginCommandBuffer(cmdBuffer[usedBuffer], &commandBufferBeginInfo);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not begin command buffer.");

		return VK_FALSE;
	}

	//

    VkImageMemoryBarrier imageMemoryBarrier;

    memset(&imageMemoryBarrier, 0, sizeof(VkImageMemoryBarrier));

    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

    imageMemoryBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.image = swapchainImage[usedBuffer];
    imageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

    vkCmdPipelineBarrier(cmdBuffer[usedBuffer], VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

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

	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.framebuffer = framebuffer[usedBuffer];
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent = swapchainCreateInfo.imageExtent;
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = clearValues;

	vkCmdBeginRenderPass(cmdBuffer[usedBuffer], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	// The window is cleaned with above defined blue color due to the VK_ATTACHMENT_LOAD_OP_CLEAR (see render pass creation).

	vkCmdEndRenderPass(cmdBuffer[usedBuffer]);

	//

	memset(&imageMemoryBarrier, 0, sizeof(VkImageMemoryBarrier));

	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

	imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	imageMemoryBarrier.image = swapchainImage[usedBuffer];
	imageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

	vkCmdPipelineBarrier(cmdBuffer[usedBuffer], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	//

	result = vkEndCommandBuffer(cmdBuffer[usedBuffer]);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not end command buffer.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildFramebuffer(const int32_t usedBuffer)
{
	VkResult result;

	//

	VkFramebufferCreateInfo framebufferCreateInfo;

	memset(&framebufferCreateInfo, 0, sizeof(VkFramebufferCreateInfo));

	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

	framebufferCreateInfo.flags = 0;
	framebufferCreateInfo.renderPass = renderPass;
	framebufferCreateInfo.attachmentCount = 1;
	framebufferCreateInfo.pAttachments = &swapchainImageView[usedBuffer];
	framebufferCreateInfo.width = (uint32_t) swapchainCreateInfo.imageExtent.width;
	framebufferCreateInfo.height = (uint32_t) swapchainCreateInfo.imageExtent.height;
	framebufferCreateInfo.layers = 1;

	result = vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &framebuffer[usedBuffer]);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create frame buffer.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildSwapchainImageView(const int32_t usedBuffer)
{
	VkResult result;

	//

	VkImageViewCreateInfo imageViewCreateInfo;

	memset(&imageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));

	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

	imageViewCreateInfo.flags = 0;
	imageViewCreateInfo.image = swapchainImage[usedBuffer];
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = swapchainCreateInfo.imageFormat;
	imageViewCreateInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
	imageViewCreateInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

	result = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &swapchainImageView[usedBuffer]);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create color attachment view.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildSwapchainImageLayout(const int32_t usedBuffer, const VkCommandBuffer cmdBuffer)
{
	VkImageMemoryBarrier imageMemoryBarrier;

	memset(&imageMemoryBarrier, 0, sizeof(VkImageMemoryBarrier));

	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

	imageMemoryBarrier.srcAccessMask = 0;
	imageMemoryBarrier.dstAccessMask = 0;
	imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	imageMemoryBarrier.srcQueueFamilyIndex = 0;
	imageMemoryBarrier.dstQueueFamilyIndex = 0;
	imageMemoryBarrier.image = swapchainImage[usedBuffer];
	imageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

	vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	return VK_TRUE;
}

VkBool32 Example::buildRenderPass()
{
	VkResult result;

	//

	VkAttachmentDescription attachmentDescription;

	memset(&attachmentDescription, 0, sizeof(VkAttachmentDescription));

	attachmentDescription.flags = 0;
	attachmentDescription.format = swapchainCreateInfo.imageFormat;
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

	VkRenderPassCreateInfo renderPassCreateInfo;

	memset(&renderPassCreateInfo, 0, sizeof(VkRenderPassCreateInfo));

	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

	renderPassCreateInfo.flags = 0;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &attachmentDescription;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.dependencyCount = 0;
	renderPassCreateInfo.pDependencies = nullptr;

	result = vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create render pass.");

		return VK_FALSE;
	}

	return VK_TRUE;
}

VkBool32 Example::buildSwapchain()
{
	VkResult result;

	//

	result = vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create swap chain.");

		return VK_FALSE;
	}

	//

	if (swapchainCreateInfo.oldSwapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(device, swapchainCreateInfo.oldSwapchain, nullptr);

		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	}

	//

	result = vkGetSwapchainImagesKHR(device, swapchain, &swapchainImagesCount, NULL);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get swap chain images size.");

		return VK_FALSE;
	}

	swapchainImage = std::vector<VkImage>(swapchainImagesCount);
    swapchainImageView = std::vector<VkImageView>(swapchainImagesCount);
    framebuffer = std::vector<VkFramebuffer>(swapchainImagesCount);
    cmdBuffer = std::vector<VkCommandBuffer>(swapchainImagesCount);

	result = vkGetSwapchainImagesKHR(device, swapchain, &swapchainImagesCount, &swapchainImage[0]);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get swap chain image info.");

		return VK_FALSE;
	}

	//

	return VK_TRUE;
}

VkBool32 Example::buildResources(const vkts::IUpdateThreadContext& updateContext)
{
	VkResult result;

	//

	VkSurfaceCapabilitiesKHR surfaceCapabilities;

	result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface->getSurface(), &surfaceCapabilities);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get surface capabilities.");

		return VK_FALSE;
	}

	VkSurfaceTransformFlagBitsKHR preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

	if (!(surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR))
	{
		preTransform = surfaceCapabilities.currentTransform;
	}

	uint32_t surfacePresentModesCount;

	result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface->getSurface(), &surfacePresentModesCount, nullptr);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get surface present modes count.");

		return VK_FALSE;
	}

	std::unique_ptr<VkPresentModeKHR[]> surfacePresentModes = std::unique_ptr<VkPresentModeKHR[]>(new VkPresentModeKHR[surfacePresentModesCount]);

	if (!surfacePresentModes.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get surface present modes.");

		return VK_FALSE;
	}

	result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface->getSurface(), &surfacePresentModesCount, &surfacePresentModes[0]);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get surface present modes.");

		return VK_FALSE;
	}

	// Regarding specification, this present mode has to be supported.
	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

	uint32_t surfaceFormatsCount;

	result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface->getSurface(), &surfaceFormatsCount, nullptr);

	if (result != VK_SUCCESS || surfaceFormatsCount == 0)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get surface formats count.");

		return VK_FALSE;
	}

	std::unique_ptr<VkSurfaceFormatKHR[]> surfaceFormats = std::unique_ptr<VkSurfaceFormatKHR[]>(new VkSurfaceFormatKHR[surfaceFormatsCount]);

	if (!surfaceFormats.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create surface formats.");

		return VK_FALSE;
	}

	result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface->getSurface(), &surfaceFormatsCount, &surfaceFormats[0]);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get surface formats.");

		return VK_FALSE;
	}

	VkFormat format = VK_FORMAT_B8G8R8A8_UNORM;

	VkColorSpaceKHR imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

	VkBool32 surfaceFormatFound = VK_FALSE;

	for (size_t i = 0; i < surfaceFormatsCount; i++)
	{
		if (surfaceFormats[i].format == format && surfaceFormats[i].colorSpace == imageColorSpace)
		{
			surfaceFormatFound = VK_TRUE;

			break;
		}
	}

	if (!surfaceFormatFound)
	{
		format = surfaceFormats[0].format;
		imageColorSpace = surfaceFormats[0].colorSpace;
	}

	memset(&swapchainCreateInfo, 0, sizeof(VkSwapchainCreateInfoKHR));

	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

	swapchainCreateInfo.flags = 0;
	swapchainCreateInfo.surface = surface->getSurface();
	swapchainCreateInfo.minImageCount = VKTS_NUMBER_BUFFERS;
	swapchainCreateInfo.imageFormat = format;
	swapchainCreateInfo.imageColorSpace = imageColorSpace;
	swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	swapchainCreateInfo.preTransform = preTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = swapchain;

	if (!buildSwapchain())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build swap chain.");

		return VK_FALSE;
	}

	if (!buildRenderPass())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build render pass.");

		return VK_FALSE;
	}

	//

	VkCommandBufferAllocateInfo cmdBufferAllocateInfo;

	memset(&cmdBufferAllocateInfo, 0, sizeof(VkCommandBufferAllocateInfo));

	cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

	cmdBufferAllocateInfo.commandPool = commandPool;
	cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmdBufferAllocateInfo.commandBufferCount = 1;

	VkCommandBuffer updateCmdBuffer;

	result = vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, &updateCmdBuffer);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create command buffer.");

		return VK_FALSE;
	}

	VkCommandBufferInheritanceInfo commandBufferInheritanceInfo;

	memset(&commandBufferInheritanceInfo, 0, sizeof(VkCommandBufferInheritanceInfo));

	commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;

	commandBufferInheritanceInfo.renderPass = VK_NULL_HANDLE;
	commandBufferInheritanceInfo.subpass = 0;
	commandBufferInheritanceInfo.framebuffer = VK_NULL_HANDLE;
	commandBufferInheritanceInfo.occlusionQueryEnable = VK_FALSE;
	commandBufferInheritanceInfo.queryFlags = 0;
	commandBufferInheritanceInfo.pipelineStatistics = 0;

	VkCommandBufferBeginInfo cmdBufferBeginInfo;

	memset(&cmdBufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));

	cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	cmdBufferBeginInfo.pInheritanceInfo = &commandBufferInheritanceInfo;

	result = vkBeginCommandBuffer(updateCmdBuffer, &cmdBufferBeginInfo);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not begin command buffer.");

		return VK_FALSE;
	}

	for (int32_t i = 0; i < (int32_t)swapchainImagesCount; i++)
	{
		if (!buildSwapchainImageLayout(i, updateCmdBuffer))
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build swap chain image layout.");

			return VK_FALSE;
		}
	}

	result = vkEndCommandBuffer(updateCmdBuffer);

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
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &updateCmdBuffer;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = nullptr;

	result = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not submit queue.");

		return VK_FALSE;
	}

	result = vkQueueWaitIdle(queue);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR,
				"Example: Could not wait for idle queue.");

		return VK_FALSE;
	}

	vkFreeCommandBuffers(device, commandPool, 1, &updateCmdBuffer);

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
	if (device != VK_NULL_HANDLE)
	{
		for (int32_t i = 0; i < (int32_t)swapchainImagesCount; i++)
		{
			if (cmdBuffer[i] != VK_NULL_HANDLE)
			{
				vkFreeCommandBuffers(device, commandPool, 1, &cmdBuffer[i]);

				cmdBuffer[i] = VK_NULL_HANDLE;
			}

			if (framebuffer[i] != VK_NULL_HANDLE)
			{
				vkDestroyFramebuffer(device, framebuffer[i], nullptr);

				framebuffer[i] = VK_NULL_HANDLE;
			}

			if (swapchainImageView[i] != VK_NULL_HANDLE)
			{
				vkDestroyImageView(device, swapchainImageView[i], nullptr);

				swapchainImageView[i] = VK_NULL_HANDLE;
			}

			swapchainImage[i] = VK_NULL_HANDLE;
		}

		if (renderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(device, renderPass, nullptr);

			renderPass = VK_NULL_HANDLE;
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
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not get window.");

		return VK_FALSE;
	}

	windowDimension = updateContext.getWindowDimension(windowIndex);

	//

	VkResult result;

	//

	VkCommandPoolCreateInfo commandPoolCreateInfo;

	memset(&commandPoolCreateInfo, 0, sizeof(VkCommandPoolCreateInfo));

	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = 0;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

	result = vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create command pool.");

		return VK_FALSE;
	}

	//

	VkSemaphoreCreateInfo semaphoreCreateInfo;

	memset(&semaphoreCreateInfo, 0, sizeof(VkSemaphoreCreateInfo));

	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	semaphoreCreateInfo.flags = 0;

	result = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAcquiredSemaphore);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create semaphore.");

		return VK_FALSE;
	}

    result = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderingCompleteSemaphore);

    if (result != VK_SUCCESS)
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create semaphore.");

        return VK_FALSE;
    }

	//

	if (!buildResources(updateContext))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not build resources.");

		return VK_FALSE;
	}

	//

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
		result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &currentBuffer);
	}

	if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
	{
		VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

		VkSubmitInfo submitInfo;

		memset(&submitInfo, 0, sizeof(VkSubmitInfo));

		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &imageAcquiredSemaphore;
		submitInfo.pWaitDstStageMask = &waitDstStageMask;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer[currentBuffer];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &renderingCompleteSemaphore;

		result = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);

		if (result != VK_SUCCESS)
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not submit queue.");

			return VK_FALSE;
		}

		VkPresentInfoKHR presentInfo;

		memset(&presentInfo, 0, sizeof(VkPresentInfoKHR));

		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderingCompleteSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &swapchain;
		presentInfo.pImageIndices = &currentBuffer;
		presentInfo.pResults = nullptr;

		result = vkQueuePresentKHR(queue, &presentInfo);

		if (result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR)
		{
			result = vkQueueWaitIdle(queue);

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

	vkDestroySemaphore(device, imageAcquiredSemaphore, nullptr);

    vkDestroySemaphore(device, renderingCompleteSemaphore, nullptr);

    //

	VkSemaphoreCreateInfo semaphoreCreateInfo;

	memset(&semaphoreCreateInfo, 0, sizeof(VkSemaphoreCreateInfo));

	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	semaphoreCreateInfo.flags = 0;

	result = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &imageAcquiredSemaphore);

	if (result != VK_SUCCESS)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create semaphore.");

		return VK_FALSE;
	}

    result = vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &renderingCompleteSemaphore);

    if (result != VK_SUCCESS)
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not create semaphore.");

        return VK_FALSE;
    }

	return VK_TRUE;
}

//
// Vulkan termination.
//
void Example::terminate(const vkts::IUpdateThreadContext& updateContext)
{
	if (device != VK_NULL_HANDLE)
	{
		terminateResources(updateContext);

		//

		if (swapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(device, swapchain, nullptr);

			swapchain = VK_NULL_HANDLE;
		}

        if (renderingCompleteSemaphore != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(device, renderingCompleteSemaphore, nullptr);

            renderingCompleteSemaphore = VK_NULL_HANDLE;
        }

		if (imageAcquiredSemaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(device, imageAcquiredSemaphore, nullptr);

			imageAcquiredSemaphore = VK_NULL_HANDLE;
		}

		if (commandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(device, commandPool, nullptr);

			commandPool = VK_NULL_HANDLE;
		}
	}
}
