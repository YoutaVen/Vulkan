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

#include "Buffer.hpp"

namespace vkts
{

Buffer::Buffer(const VkDevice device, const VkBufferCreateFlags flags, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkSharingMode sharingMode, const uint32_t queueFamilyIndexCount, const uint32_t* queueFamilyIndices, const VkBuffer buffer) :
    IBuffer(), device(device), bufferCreateInfo{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr, flags, size, usage, sharingMode, queueFamilyIndexCount, nullptr}, buffer(buffer)
{
    for (uint32_t i = 0; i < queueFamilyIndexCount; i++)
    {
        allQueueFamilyIndices.push_back(queueFamilyIndices[i]);
    }

    if (queueFamilyIndexCount > 0)
    {
        bufferCreateInfo.pQueueFamilyIndices = &allQueueFamilyIndices[0];
    }
}

Buffer::~Buffer()
{
    destroy();
}

//
// IGpuMemory
//

const VkDevice Buffer::getDevice() const
{
    return device;
}

const VkBufferCreateInfo& Buffer::getBufferCreateInfo() const
{
    return bufferCreateInfo;
}

VkBufferCreateFlags Buffer::getFlags() const
{
    return bufferCreateInfo.flags;
}

VkDeviceSize Buffer::getSize() const
{
    return bufferCreateInfo.size;
}

VkBufferUsageFlags Buffer::getUsage() const
{
    return bufferCreateInfo.usage;
}

VkSharingMode Buffer::getSharingMode() const
{
    return bufferCreateInfo.sharingMode;
}

uint32_t Buffer::getQueueFamilyIndexCount() const
{
    return bufferCreateInfo.queueFamilyIndexCount;
}

const uint32_t* Buffer::getQueueFamilyIndices() const
{
    return bufferCreateInfo.pQueueFamilyIndices;
}

const VkBuffer Buffer::getBuffer() const
{
    return buffer;
}

void Buffer::getBufferMemoryRequirements(VkMemoryRequirements& memoryRequirements) const
{
    vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);
}

VkResult Buffer::bindBufferMemory(const VkDeviceMemory mem, const VkDeviceSize memOffset) const
{
    return vkBindBufferMemory(device, buffer, mem, memOffset);
}

void Buffer::copyBuffer(const VkCommandBuffer cmdBuffer, const VkBuffer targetBuffer, const VkDeviceSize targetBufferSize, const uint32_t regionCount, const VkBufferCopy* bufferCopy) const
{
    vkCmdCopyBuffer(cmdBuffer, buffer, targetBuffer, regionCount, bufferCopy);
}

void Buffer::copyBufferToImage(const VkCommandBuffer cmdBuffer, const VkImage targetImage, const VkImageLayout targetImageLayout, const uint32_t regionCount, const VkBufferImageCopy* regions) const
{
    vkCmdCopyBufferToImage(cmdBuffer, buffer, targetImage, targetImageLayout, regionCount, regions);
}

//
// IDestroyable
//

void Buffer::destroy()
{
    if (buffer)
    {
        vkDestroyBuffer(device, buffer, nullptr);

        buffer = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
