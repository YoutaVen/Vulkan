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

#ifndef VKTS_IMAGE_HPP_
#define VKTS_IMAGE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Image: public IImage
{

private:

    const VkDevice device;

    VkImageCreateInfo imageCreateInfo;

    std::vector<uint32_t> allQueueFamilyIndices;

    VkImageLayout imageLayout;

    VkAccessFlags accessMask;

    VkImage image;

public:

    Image() = delete;
    Image(const VkDevice device, const VkImageCreateFlags flags, const VkImageType imageType, const VkFormat format, const VkExtent3D& extent, const uint32_t mipLevels, const uint32_t arrayLayers, const VkSampleCountFlagBits samples, const VkImageTiling tiling, const VkImageUsageFlags usage, const VkSharingMode sharingMode, const uint32_t queueFamilyIndexCount, const uint32_t* queueFamilyIndices, const VkImageLayout initialLayout, const VkAccessFlags accessMask, const VkImage image);
    Image(const Image& other) = delete;
    Image(Image&& other) = delete;
    virtual ~Image();

    Image& operator =(const Image& other) = delete;

    Image& operator =(Image && other) = delete;

    //
    // IImage
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkImageCreateInfo& getImageCreateInfo() const override;

    virtual VkImageCreateFlags getFlags() const override;

    virtual VkImageType getImageType() const override;

    virtual const VkFormat& getFormat() const override;

    virtual const VkExtent3D& getExtent() const override;

    virtual int32_t getWidth() const override;

    virtual int32_t getHeight() const override;

    virtual int32_t getDepth() const override;

    virtual uint32_t getMipLevels() const override;

    virtual uint32_t getArrayLayers() const override;

    virtual VkSampleCountFlagBits getSamples() const override;

    virtual VkImageTiling getTiling() const override;

    virtual VkImageUsageFlags getUsage() const override;

    virtual VkSharingMode getSharingMode() const override;

    virtual uint32_t getQueueFamilyCount() const override;

    virtual const uint32_t* getQueueFamilyIndices() const override;

    virtual VkImageLayout getInitialLayout() const override;

    virtual const VkImage getImage() const override;

    virtual VkAccessFlags getAccessMask() const override;

    virtual VkImageLayout getImageLayout() const override;

    virtual void getImageMemoryRequirements(VkMemoryRequirements& memoryRequirements) const override;

    virtual void getImageSubresourceLayout(VkSubresourceLayout& subresourceLayout, const VkImageSubresource& imageSubresource) const override;

    virtual void copyImage(const VkCommandBuffer cmdBuffer, const VkImage targetImage, const VkAccessFlags targetAccessMask, const VkImageLayout targetImageLayout, const VkImageCopy& iamgeCopy) const override;

    virtual void copyImageToBuffer(const VkCommandBuffer cmdBuffer, const VkBuffer dstBuffer, const uint32_t regionCount, const VkBufferImageCopy* regions, const VkImageSubresourceRange& subresourceRange) const override;

    virtual void cmdPipelineBarrier(const VkCommandBuffer cmdBuffer, const VkAccessFlags dstAccessMask, const VkImageLayout newLayout, const VkImageSubresourceRange& subresourceRange) override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_IMAGE_HPP_ */
