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

#include "BufferView.hpp"

namespace vkts
{

BufferView::BufferView(const VkDevice device, const VkBufferViewCreateFlags flags, const VkBuffer buffer, const VkFormat format, const VkDeviceSize offset, const VkDeviceSize range, const VkBufferView bufferView) :
    IBufferView(), device(device), bufferViewCreateInfo{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO, nullptr, flags, buffer, format, offset, range}, bufferView(bufferView)
{
}

BufferView::~BufferView()
{
    destroy();
}

//
// IGpuMemory
//

const VkDevice BufferView::getDevice() const
{
    return device;
}

const VkBufferViewCreateInfo& BufferView::getBufferViewCreateInfo() const
{
    return bufferViewCreateInfo;
}

VkBufferViewCreateFlags BufferView::getFlags() const
{
    return bufferViewCreateInfo.flags;
}

const VkBuffer BufferView::getBuffer() const
{
    return bufferViewCreateInfo.buffer;
}

VkFormat BufferView::getFormat() const
{
    return bufferViewCreateInfo.format;
}

VkDeviceSize BufferView::getOffset() const
{
    return bufferViewCreateInfo.offset;
}

VkDeviceSize BufferView::getRange() const
{
    return bufferViewCreateInfo.range;
}

const VkBufferView BufferView::getBufferView() const
{
    return bufferView;
}

//
// IDestroyable
//

void BufferView::destroy()
{
    if (bufferView)
    {
        vkDestroyBufferView(device, bufferView, nullptr);

        bufferView = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
