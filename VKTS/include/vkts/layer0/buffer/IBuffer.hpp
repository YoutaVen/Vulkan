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

#ifndef VKTS_IBUFFER_HPP_
#define VKTS_IBUFFER_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IBuffer: public IDestroyable
{

public:

    IBuffer() :
        IDestroyable()
    {
    }

    virtual ~IBuffer()
    {
    }

    virtual const VkDevice getDevice() const = 0;

    virtual const VkBufferCreateInfo& getBufferCreateInfo() const = 0;

    virtual VkBufferCreateFlags getFlags() const = 0;

    virtual VkDeviceSize getSize() const = 0;

    virtual VkBufferUsageFlags getUsage() const = 0;

    virtual VkSharingMode getSharingMode() const = 0;

    virtual uint32_t getQueueFamilyIndexCount() const = 0;

    virtual const uint32_t* getQueueFamilyIndices() const = 0;

    virtual const VkBuffer getBuffer() const = 0;

    virtual void getBufferMemoryRequirements(VkMemoryRequirements& memoryRequirements) const = 0;

    virtual VkResult bindBufferMemory(const VkDeviceMemory mem, const VkDeviceSize memOffset) const = 0;

    virtual void copyBuffer(const VkCommandBuffer cmdBuffer, const VkBuffer targetBuffer, const VkDeviceSize targetBufferSize, const uint32_t regionCount, const VkBufferCopy* bufferCopy) const = 0;

    virtual void copyBufferToImage(const VkCommandBuffer cmdBuffer, const VkImage targetImage, const VkImageLayout targetImageLayout, const uint32_t regionCount, const VkBufferImageCopy* regions) const = 0;

};

typedef std::shared_ptr<IBuffer> IBufferSP;

} /* namespace vkts */

#endif /* VKTS_IBUFFER_HPP_ */
