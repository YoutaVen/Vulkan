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

#ifndef VKTS_BUFFER_HPP_
#define VKTS_BUFFER_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Buffer: public IBuffer
{

private:

    const VkDevice device;

    VkBufferCreateInfo bufferCreateInfo;

    std::vector<uint32_t> allQueueFamilyIndices;

    VkBuffer buffer;

public:

    Buffer() = delete;
    Buffer(const VkDevice device, const VkBufferCreateFlags flags, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkSharingMode sharingMode, const uint32_t queueFamilyIndexCount, const uint32_t* queueFamilyIndices, const VkBuffer buffer);
    Buffer(const Buffer& other) = delete;
    Buffer(Buffer&& other) = delete;
    virtual ~Buffer();

    Buffer& operator =(const Buffer& other) = delete;

    Buffer& operator =(Buffer && other) = delete;

    //
    // IBuffer
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkBufferCreateInfo& getBufferCreateInfo() const override;

    virtual VkBufferCreateFlags getFlags() const override;

    virtual VkDeviceSize getSize() const override;

    virtual VkBufferUsageFlags getUsage() const override;

    virtual VkSharingMode getSharingMode() const override;

    virtual uint32_t getQueueFamilyIndexCount() const override;

    virtual const uint32_t* getQueueFamilyIndices() const override;

    virtual const VkBuffer getBuffer() const override;

    virtual void getBufferMemoryRequirements(VkMemoryRequirements& memoryRequirements) const override;

    virtual VkResult bindBufferMemory(const VkDeviceMemory mem, const VkDeviceSize memOffset) const override;

    virtual void copyBuffer(const VkCommandBuffer cmdBuffer, const VkBuffer targetBuffer, const VkDeviceSize targetBufferSize, const uint32_t regionCount, const VkBufferCopy* bufferCopy) const override;

    virtual void copyBufferToImage(const VkCommandBuffer cmdBuffer, const VkImage targetImage, const VkImageLayout targetImageLayout, const uint32_t regionCount, const VkBufferImageCopy* regions) const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_BUFFER_HPP_ */
