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

#ifndef VKTS_SWAPCHAIN_HPP_
#define VKTS_SWAPCHAIN_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Swapchain: public ISwapchain
{

private:

    const VkDevice device;

    VkSwapchainCreateInfoKHR swapchainCreateInfo;

    std::vector<uint32_t> queueFamilyIndices;

    VkSwapchainKHR swapchain;

    std::vector<VkImage> allSwapchainImages;
    std::vector<VkAccessFlags> allSwapchainAccessFlags;
    std::vector<VkImageLayout> allSwapchainImageLayouts;

public:

    Swapchain() = delete;
    Swapchain(const VkDevice device, const VkSwapchainCreateFlagsKHR flags, const VkSurfaceKHR surface, const uint32_t minImageCount, const VkFormat imageFormat, const VkColorSpaceKHR imageColorSpace, const VkExtent2D imageExtent, const uint32_t imageArrayLayers, const VkImageUsageFlags imageUsage, const VkSharingMode imageSharingMode, const uint32_t queueFamilyIndexCount, const uint32_t* pQueueFamilyIndices, const VkSurfaceTransformFlagBitsKHR preTransform, const VkCompositeAlphaFlagBitsKHR compositeAlpha, const VkPresentModeKHR presentMode, const VkBool32 clipped, const VkSwapchainKHR oldSwapchain, const VkSwapchainKHR swapchain);
    Swapchain(const Swapchain& other) = delete;
    Swapchain(Swapchain&& other) = delete;
    virtual ~Swapchain();

    Swapchain& operator =(const Swapchain& other) = delete;
    Swapchain& operator =(Swapchain && other) = delete;

    //
    // ISwapchain
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkSwapchainCreateInfoKHR& getSwapchainCreateInfo() const override;

    virtual VkSwapchainCreateFlagsKHR getFlags() const override;

    virtual VkSurfaceKHR getSurface() const override;

    virtual uint32_t getMinImageCount() const override;

    virtual VkFormat getImageFormat() const override;

    virtual VkColorSpaceKHR getImageColorSpace() const override;

    virtual const VkExtent2D& getImageExtent() const override;

    virtual uint32_t getImageArrayLayers() const override;

    virtual VkImageUsageFlags getImageUsage() const override;

    virtual VkSharingMode getImageSharingMode() const override;

    virtual uint32_t getQueueFamilyIndexCount() const override;

    virtual const uint32_t* getQueueFamilyIndices() const override;

    virtual VkSurfaceTransformFlagBitsKHR getPreTransform() const override;

    virtual VkCompositeAlphaFlagBitsKHR getCompositeAlpha() const override;

    virtual VkPresentModeKHR getPresentMode() const override;

    virtual VkBool32 getClipped() const override;

    virtual const VkSwapchainKHR getOldSwapchain() const override;

    virtual const VkSwapchainKHR getSwapchain() const override;

    virtual const std::vector<VkImage>& getAllSwapchainImages(const VkBool32 refresh = VK_FALSE) override;

    virtual const VkImage getSwapchainImage(const uint32_t index, const VkBool32 refresh = VK_FALSE) override;

    virtual void cmdPipelineBarrier(const VkCommandBuffer cmdBuffer, const VkAccessFlags dstAccessMask, const VkImageLayout newLayout, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const uint32_t index, const VkBool32 refresh = VK_FALSE) override;

    virtual VkResult acquireNextImage(const uint64_t timeout, const VkSemaphore semaphore, const VkFence fence, uint32_t& pImageIndex) const override;

    virtual VkResult queuePresent(const VkQueue queue, const uint32_t waitSemaphoreCount, const VkSemaphore* waitSemaphores, const uint32_t swapchainCount, const VkSwapchainKHR* swapchains, const uint32_t* imageIndices, VkResult* results) const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_SWAPCHAIN_HPP_ */
