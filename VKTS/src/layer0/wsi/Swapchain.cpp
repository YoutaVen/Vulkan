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

#include "Swapchain.hpp"

namespace vkts
{

Swapchain::Swapchain(const VkDevice device, const VkSwapchainCreateFlagsKHR flags, const VkSurfaceKHR surface, const uint32_t minImageCount, const VkFormat imageFormat, const VkColorSpaceKHR imageColorSpace, const VkExtent2D imageExtent, const uint32_t imageArrayLayers, const VkImageUsageFlags imageUsage, const VkSharingMode imageSharingMode, const uint32_t queueFamilyIndexCount, const uint32_t* pQueueFamilyIndices, const VkSurfaceTransformFlagBitsKHR preTransform, const VkCompositeAlphaFlagBitsKHR compositeAlpha, const VkPresentModeKHR presentMode, const VkBool32 clipped, const VkSwapchainKHR oldSwapchain, const VkSwapchainKHR swapchain) :
    ISwapchain(), device(device), swapchainCreateInfo{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, nullptr, flags, surface, minImageCount, imageFormat, imageColorSpace, imageExtent, imageArrayLayers, imageUsage, imageSharingMode, queueFamilyIndexCount, nullptr, preTransform, compositeAlpha, presentMode, clipped, oldSwapchain}, swapchain(swapchain), allSwapchainImages(), allSwapchainAccessFlags(), allSwapchainImageLayouts()
{
    if (queueFamilyIndexCount > 0 && queueFamilyIndices.size() > 0)
    {
        for (uint32_t i = 0; i < queueFamilyIndexCount; i++)
        {
            this->queueFamilyIndices.push_back(queueFamilyIndices[i]);
        }

        swapchainCreateInfo.pQueueFamilyIndices = &this->queueFamilyIndices[0];
    }
}

Swapchain::~Swapchain()
{
    destroy();
}

//
// ISwapchain
//

const VkDevice Swapchain::getDevice() const
{
    return device;
}

const VkSwapchainCreateInfoKHR& Swapchain::getSwapchainCreateInfo() const
{
    return swapchainCreateInfo;
}

VkSwapchainCreateFlagsKHR Swapchain::getFlags() const
{
    return swapchainCreateInfo.flags;
}

VkSurfaceKHR Swapchain::getSurface() const
{
    return swapchainCreateInfo.surface;
}

uint32_t Swapchain::getMinImageCount() const
{
    return swapchainCreateInfo.minImageCount;
}

VkFormat Swapchain::getImageFormat() const
{
    return swapchainCreateInfo.imageFormat;
}

VkColorSpaceKHR Swapchain::getImageColorSpace() const
{
    return swapchainCreateInfo.imageColorSpace;
}

const VkExtent2D& Swapchain::getImageExtent() const
{
    return swapchainCreateInfo.imageExtent;
}

uint32_t Swapchain::getImageArrayLayers() const
{
    return swapchainCreateInfo.imageArrayLayers;
}

VkFlags Swapchain::getImageUsage() const
{
    return swapchainCreateInfo.imageUsage;
}

VkSharingMode Swapchain::getImageSharingMode() const
{
    return swapchainCreateInfo.imageSharingMode;
}

uint32_t Swapchain::getQueueFamilyIndexCount() const
{
    return swapchainCreateInfo.queueFamilyIndexCount;
}

const uint32_t* Swapchain::getQueueFamilyIndices() const
{
    return swapchainCreateInfo.pQueueFamilyIndices;
}

VkSurfaceTransformFlagBitsKHR Swapchain::getPreTransform() const
{
    return swapchainCreateInfo.preTransform;
}

VkCompositeAlphaFlagBitsKHR Swapchain::getCompositeAlpha() const
{
    return swapchainCreateInfo.compositeAlpha;
}

VkPresentModeKHR Swapchain::getPresentMode() const
{
    return swapchainCreateInfo.presentMode;
}

VkBool32 Swapchain::getClipped() const
{
    return swapchainCreateInfo.clipped;
}

const VkSwapchainKHR Swapchain::getOldSwapchain() const
{
    return swapchainCreateInfo.oldSwapchain;
}

const VkSwapchainKHR Swapchain::getSwapchain() const
{
    return swapchain;
}

const std::vector<VkImage>& Swapchain::getAllSwapchainImages(const VkBool32 refresh)
{
    if (allSwapchainImages.size() == 0 || refresh)
    {
        allSwapchainImages.clear();
        allSwapchainAccessFlags.clear();
        allSwapchainImageLayouts.clear();

        VkResult result;

        uint32_t swapchainImagesCount;

        result = vkGetSwapchainImagesKHR(device, swapchain, &swapchainImagesCount, nullptr);

        if (result != VK_SUCCESS)
        {
            logPrint(VKTS_LOG_ERROR, "Swapchain: Could not get swapchain images count.");

            return allSwapchainImages;
        }

        allSwapchainImages = std::vector<VkImage>(swapchainImagesCount);
        allSwapchainAccessFlags = std::vector<VkAccessFlags>(swapchainImagesCount);
        allSwapchainImageLayouts = std::vector<VkImageLayout>(swapchainImagesCount);

        result = vkGetSwapchainImagesKHR(device, swapchain, &swapchainImagesCount, &allSwapchainImages[0]);

        if (result != VK_SUCCESS)
        {
            logPrint(VKTS_LOG_ERROR, "Swapchain: Could not get swapchain image info.");

            allSwapchainImages.clear();
            allSwapchainAccessFlags.clear();
            allSwapchainImageLayouts.clear();
        }
    }

    return allSwapchainImages;
}

const VkImage Swapchain::getSwapchainImage(const uint32_t index, const VkBool32 refresh)
{
    if (allSwapchainImages.size() == 0 || refresh)
    {
        getAllSwapchainImages(refresh);
    }

    if (index < allSwapchainImages.size())
    {
        return allSwapchainImages[index];
    }

    return VK_NULL_HANDLE;
}

void Swapchain::cmdPipelineBarrier(const VkCommandBuffer cmdBuffer, const VkAccessFlags dstAccessMask, const VkImageLayout newLayout, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const uint32_t index, const VkBool32 refresh)
{
	if (allSwapchainImages.size() == 0 || refresh)
	{
		getAllSwapchainImages(VK_TRUE);
	}

    if (index >= allSwapchainImages.size())
    {
        return;
    }

	VkImageMemoryBarrier imageMemoryBarrier;

	memset(&imageMemoryBarrier, 0, sizeof(VkImageMemoryBarrier));

	imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

	imageMemoryBarrier.srcAccessMask = allSwapchainAccessFlags[index];
	imageMemoryBarrier.dstAccessMask = dstAccessMask;
	imageMemoryBarrier.oldLayout = allSwapchainImageLayouts[index];
	imageMemoryBarrier.newLayout = newLayout;
	imageMemoryBarrier.srcQueueFamilyIndex = 0;
	imageMemoryBarrier.dstQueueFamilyIndex = 0;
	imageMemoryBarrier.image = allSwapchainImages[index];
	imageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};

	vkCmdPipelineBarrier(cmdBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	allSwapchainAccessFlags[index] = dstAccessMask;
	allSwapchainImageLayouts[index] = newLayout;
}

VkResult Swapchain::acquireNextImage(const uint64_t timeout, const VkSemaphore semaphore, const VkFence fence, uint32_t& pImageIndex) const
{
    return vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, &pImageIndex);
}

VkResult Swapchain::queuePresent(const VkQueue queue, const uint32_t waitSemaphoreCount, const VkSemaphore* waitSemaphores, const uint32_t swapchainCount, const VkSwapchainKHR* swapchains, const uint32_t* imageIndices, VkResult* results) const
{
    VkPresentInfoKHR presentInfo;

    memset(&presentInfo, 0, sizeof(VkPresentInfoKHR));

    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = waitSemaphoreCount;
    presentInfo.pWaitSemaphores = waitSemaphores;
    presentInfo.swapchainCount = swapchainCount;
    presentInfo.pSwapchains = swapchains;
    presentInfo.pImageIndices = imageIndices;
    presentInfo.pResults = results;

    return vkQueuePresentKHR(queue, &presentInfo);
}

//
// IDestroyable
//

void Swapchain::destroy()
{
    if (swapchain)
    {
        vkDestroySwapchainKHR(device, swapchain, nullptr);

        swapchain = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
