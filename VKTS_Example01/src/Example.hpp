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

#define VKTS_EXAMPLE_NAME "Example01"

#define VKTS_NUMBER_BUFFERS 2

class Example: public vkts::IUpdateThread
{

private:

	const VkInstance instance;

	const VkPhysicalDevice physicalDevice;

	const int32_t windowIndex;

	const vkts::ISurfaceSP surface;

	const VkDevice device;

	const uint32_t queueFamilyIndex;

	const VkQueue queue;

	glm::uvec2 windowDimension;

	VkCommandPool commandPool;

	VkSemaphore imageAcquiredSemaphore;
    VkSemaphore renderingCompleteSemaphore;

	VkSwapchainCreateInfoKHR swapchainCreateInfo;

	VkSwapchainKHR swapchain;

	VkRenderPass renderPass;

	uint32_t swapchainImagesCount;

	std::vector<VkImage> swapchainImage;

	std::vector<VkImageView> swapchainImageView;

	std::vector<VkFramebuffer> framebuffer;

	std::vector<VkCommandBuffer> cmdBuffer;

	VkBool32 buildCmdBuffer(const int32_t usedBuffer);

	VkBool32 buildFramebuffer(const int32_t usedBuffer);

	VkBool32 buildSwapchainImageView(const int32_t usedBuffer);

	VkBool32 buildSwapchainImageLayout(const int32_t usedBuffer, const VkCommandBuffer cmdBuffer);

	VkBool32 buildRenderPass();

	VkBool32 buildSwapchain();

	VkBool32 buildResources(const vkts::IUpdateThreadContext& updateContext);

	void terminateResources(const vkts::IUpdateThreadContext& updateContext);

public:

	Example(const VkInstance instance, const VkPhysicalDevice physicalDevice, const int32_t windowIndex, const vkts::ISurfaceSP& surface, const VkDevice device, const uint32_t queueFamilyIndex, const VkQueue queue);

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
