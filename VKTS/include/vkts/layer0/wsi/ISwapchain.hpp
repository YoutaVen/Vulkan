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

#ifndef VKTS_ISWAPCHAIN_HPP_
#define VKTS_ISWAPCHAIN_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class ISwapchain: public IDestroyable
{

public:

    ISwapchain() :
        IDestroyable()
    {
    }

    virtual ~ISwapchain()
    {
    }

    virtual const VkDevice getDevice() const = 0;

    virtual const VkSwapchainCreateInfoKHR& getSwapchainCreateInfo() const = 0;

    virtual VkSwapchainCreateFlagsKHR getFlags() const = 0;

    virtual VkSurfaceKHR getSurface() const = 0;

    virtual uint32_t getMinImageCount() const = 0;

    virtual VkFormat getImageFormat() const = 0;

    virtual VkColorSpaceKHR getImageColorSpace() const = 0;

    virtual const VkExtent2D& getImageExtent() const = 0;

    virtual uint32_t getImageArrayLayers() const = 0;

    virtual VkImageUsageFlags getImageUsage() const = 0;

    virtual VkSharingMode getImageSharingMode() const = 0;

    virtual uint32_t getQueueFamilyIndexCount() const = 0;

    virtual const uint32_t* getQueueFamilyIndices() const = 0;

    virtual VkSurfaceTransformFlagBitsKHR getPreTransform() const = 0;

    virtual VkCompositeAlphaFlagBitsKHR getCompositeAlpha() const = 0;

    virtual VkPresentModeKHR getPresentMode() const = 0;

    virtual VkBool32 getClipped() const = 0;

    virtual const VkSwapchainKHR getOldSwapchain() const = 0;

    virtual const VkSwapchainKHR getSwapchain() const = 0;

    virtual const std::vector<VkImage>& getAllSwapchainImages(const VkBool32 refresh = VK_FALSE) = 0;

    virtual const VkImage getSwapchainImage(const uint32_t index, const VkBool32 refresh = VK_FALSE) = 0;

    virtual void cmdPipelineBarrier(const VkCommandBuffer cmdBuffer, const VkAccessFlags dstAccessMask, const VkImageLayout newLayout, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const uint32_t index, const VkBool32 refresh = VK_FALSE) = 0;

    virtual VkResult acquireNextImage(const uint64_t timeout, const VkSemaphore semaphore, const VkFence fence, uint32_t& pImageIndex) const = 0;

    virtual VkResult queuePresent(const VkQueue queue, const uint32_t waitSemaphoreCount, const VkSemaphore* waitSemaphores, const uint32_t swapchainCount, const VkSwapchainKHR* swapchains, const uint32_t* imageIndices, VkResult* results) const = 0;

};

typedef std::shared_ptr<ISwapchain> ISwapchainSP;

} /* namespace vkts */

#endif /* VKTS_ISWAPCHAIN_HPP_ */
