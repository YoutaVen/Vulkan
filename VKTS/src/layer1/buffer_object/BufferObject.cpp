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

#include "BufferObject.hpp"

namespace vkts
{

BufferObject::BufferObject(const IInitialResourcesSP& initialResources, const IBufferSP& buffer, const IBufferViewSP& bufferView, const IDeviceMemorySP& deviceMemory) :
    IBufferObject(), initialResources(initialResources), buffer(buffer), bufferView(bufferView), deviceMemory(deviceMemory)
{
}

BufferObject::~BufferObject()
{
    destroy();
}

//
// IBufferObject
//

const IInitialResourcesSP& BufferObject::getInitialResources() const
{
    return initialResources;
}

const IBufferSP& BufferObject::getBuffer() const
{
    return buffer;
}

const IBufferViewSP& BufferObject::getBufferView() const
{
    return bufferView;
}

const IDeviceMemorySP& BufferObject::getDeviceMemory() const
{
    return deviceMemory;
}

VkBool32 BufferObject::upload(const size_t offset, const VkMemoryMapFlags flags, const void* data, const size_t size) const
{
    if (!data || size == 0)
    {
        return VK_FALSE;
    }

    VkResult result;

    result = deviceMemory->upload(offset, deviceMemory->getAllocationSize() - offset, flags, data, size);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "BufferObject: Could not unmap memory.");

        return VK_FALSE;
    }

    return VK_TRUE;
}

VkBool32 BufferObject::upload(const size_t offset, const VkMemoryMapFlags flags, const glm::mat4& mat) const
{
    return upload(offset, flags, glm::value_ptr(mat), sizeof(float) * 16);
}

VkBool32 BufferObject::upload(const size_t offset, const VkMemoryMapFlags flags, const glm::mat3& mat) const
{
    // Only upload relevant columns and rows.
    return upload(offset, flags, glm::value_ptr(glm::mat4(mat)), sizeof(float) * 11);
}

VkBool32 BufferObject::upload(const size_t offset, const VkMemoryMapFlags flags, const glm::mat2& mat) const
{
    // Only upload relevant columns and rows.
    return upload(offset, flags, glm::value_ptr(glm::mat4(mat)), sizeof(float) * 6);
}

VkBool32 BufferObject::upload(const size_t offset, const VkMemoryMapFlags flags, const glm::vec4& vec) const
{
    return upload(offset, flags, glm::value_ptr(vec), sizeof(float) * 4);
}

VkBool32 BufferObject::upload(const size_t offset, const VkMemoryMapFlags flags, const glm::vec3& vec) const
{
    return upload(offset, flags, glm::value_ptr(vec), sizeof(float) * 3);
}

VkBool32 BufferObject::upload(const size_t offset, const VkMemoryMapFlags flags, const glm::vec2& vec) const
{
    return upload(offset, flags, glm::value_ptr(vec), sizeof(float) * 2);
}

VkBool32 BufferObject::upload(const size_t offset, const VkMemoryMapFlags flags, const float scalar) const
{
    return upload(offset, flags, &scalar, sizeof(float));
}

//
// IDestroyable
//

void BufferObject::destroy()
{
    if (bufferView.get())
    {
        bufferView->destroy();
    }

    if (buffer.get())
    {
        buffer->destroy();

        if (deviceMemory.get())
        {
            deviceMemory->destroy();
        }
    }
}

} /* namespace vkts */
