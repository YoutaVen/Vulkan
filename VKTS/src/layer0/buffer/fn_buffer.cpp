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

#include <vkts/vkts.hpp>

#include "Buffer.hpp"
#include "BufferView.hpp"

namespace vkts
{

IBufferSP VKTS_APIENTRY bufferCreate(const VkDevice device, const VkBufferCreateFlags flags, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkSharingMode sharingMode, const uint32_t queueFamilyIndexCount, const uint32_t* queueFamilyIndices)
{
    if (!device)
    {
        return IBufferSP();
    }

    VkResult result;

    VkBufferCreateInfo bufferCreateInfo;

    memset(&bufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    bufferCreateInfo.flags = flags;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = sharingMode;
    bufferCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
    bufferCreateInfo.pQueueFamilyIndices = queueFamilyIndices;

    VkBuffer buffer;

    result = vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer);
    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Buffer: Could not create buffer.");

        return IBufferSP();
    }

    auto newInstance = new Buffer(device, flags, size, usage, sharingMode,
                                  queueFamilyIndexCount, queueFamilyIndices, buffer);

    if (!newInstance)
    {
        vkDestroyBuffer(device, buffer, nullptr);

        return IBufferSP();
    }

    return IBufferSP(newInstance);
}

IBufferViewSP VKTS_APIENTRY bufferViewCreate(const VkDevice device, const VkBufferViewCreateFlags flags, const VkBuffer buffer, const VkFormat format, const VkDeviceSize offset, const VkDeviceSize range)
{
    if (!device || !buffer)
    {
        return IBufferViewSP();
    }

    VkResult result;

    VkBufferViewCreateInfo bufferViewCreateInfo;

    memset(&bufferViewCreateInfo, 0, sizeof(VkBufferViewCreateInfo));

    bufferViewCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;

    bufferViewCreateInfo.flags = flags;
    bufferViewCreateInfo.buffer = buffer;
    bufferViewCreateInfo.format = format;
    bufferViewCreateInfo.offset = offset;
    bufferViewCreateInfo.range = range;

    VkBufferView bufferView;

    result = vkCreateBufferView(device, &bufferViewCreateInfo, nullptr,
                                &bufferView);
    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "BufferView: Could not create buffer view.");

        return IBufferViewSP();
    }

    auto newInstance = new BufferView(device, flags, buffer, format, offset,
                                      range, bufferView);

    if (!newInstance)
    {
        vkDestroyBufferView(device, bufferView, nullptr);

        return IBufferViewSP();
    }

    return IBufferViewSP(newInstance);
}

}
