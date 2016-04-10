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

#ifndef VKTS_IIMAGE_HPP_
#define VKTS_IIMAGE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IImage: public IDestroyable
{

public:

    IImage() :
        IDestroyable()
    {
    }

    virtual ~IImage()
    {
    }

    virtual const VkDevice getDevice() const = 0;

    virtual const VkImageCreateInfo& getImageCreateInfo() const = 0;

    virtual VkImageCreateFlags getFlags() const = 0;

    virtual VkImageType getImageType() const = 0;

    virtual const VkFormat& getFormat() const = 0;

    virtual const VkExtent3D& getExtent() const = 0;

    virtual int32_t getWidth() const = 0;

    virtual int32_t getHeight() const = 0;

    virtual int32_t getDepth() const = 0;

    virtual uint32_t getMipLevels() const = 0;

    virtual uint32_t getArrayLayers() const = 0;

    virtual VkSampleCountFlagBits getSamples() const = 0;

    virtual VkImageTiling getTiling() const = 0;

    virtual VkImageUsageFlags getUsage() const = 0;

    virtual VkSharingMode getSharingMode() const = 0;

    virtual uint32_t getQueueFamilyCount() const = 0;

    virtual const uint32_t* getQueueFamilyIndices() const = 0;

    virtual VkImageLayout getInitialLayout() const = 0;

    virtual const VkImage getImage() const = 0;

    virtual VkAccessFlags getAccessMask() const = 0;

    virtual VkImageLayout getImageLayout() const = 0;

    virtual void getImageMemoryRequirements(VkMemoryRequirements& memoryRequirements) const = 0;

    virtual void getImageSubresourceLayout(VkSubresourceLayout& subresourceLayout, const VkImageSubresource& imageSubresource) const = 0;

    virtual void copyImage(const VkCommandBuffer cmdBuffer, const VkImage targetImage, const VkAccessFlags targetAccessMask, const VkImageLayout targetImageLayout, const VkImageCopy& imageCopy) const = 0;

    virtual void copyImageToBuffer(const VkCommandBuffer cmdBuffer, const VkBuffer dstBuffer, const uint32_t regionCount, const VkBufferImageCopy* regions, const VkImageSubresourceRange& subresourceRange) const = 0;

    virtual void cmdPipelineBarrier(const VkCommandBuffer cmdBuffer, const VkAccessFlags dstAccessMask, const VkImageLayout newLayout, const VkImageSubresourceRange& subresourceRange) = 0;

    //
    // IDestroyable
    //

    virtual void destroy() = 0;

};

typedef std::shared_ptr<IImage> IImageSP;

} /* namespace vkts */

#endif /* VKTS_IIMAGE_HPP_ */
