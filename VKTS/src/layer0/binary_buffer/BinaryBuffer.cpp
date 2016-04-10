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

#include "BinaryBuffer.hpp"

namespace vkts
{

uint8_t* BinaryBuffer::copyData(const uint8_t* source, const size_t size) const
{
    if (!source || size == 0)
    {
        return nullptr;
    }

    auto target = new uint8_t[size];

    if (!target)
    {
        return nullptr;
    }

    memcpy(target, source, size);

    return target;
}

BinaryBuffer::BinaryBuffer() :
    IBinaryBuffer(), data(nullptr), size(0), pos(0)
{
}

BinaryBuffer::BinaryBuffer(const size_t size) :
    IBinaryBuffer(), data(nullptr), size(size), pos(0)
{
    data = new uint8_t[size];

    if (!data)
    {
        this->size = 0;
    }
}

BinaryBuffer::BinaryBuffer(const uint8_t* data, const size_t size) :
    IBinaryBuffer(), data(nullptr), size(size), pos(0)
{
    this->data = copyData(data, size);

    if (!this->data)
    {
        reset();
    }
}

BinaryBuffer::~BinaryBuffer()
{
    reset();
}

//
// IBinaryBuffer
//

void BinaryBuffer::reset()
{
    size = 0;

    if (data)
    {
        delete[] data;
    }

    data = nullptr;

    pos = 0;
}

const void* BinaryBuffer::getData() const
{
    return static_cast<const void*>(data);
}

size_t BinaryBuffer::getSize() const
{
    return size;
}

VkBool32 BinaryBuffer::seek(const int64_t offset, const VkTsSearch search)
{
    if (!data)
    {
        return VK_FALSE;
    }

    switch (search)
    {
        case VKTS_SEARCH_ABSOLUTE:
        {
            if (offset < 0 || static_cast<size_t>(offset) > size)
            {
                return VK_FALSE;
            }

            pos = offset;

            return VK_TRUE;
        }
        break;
        case VKTS_SEARCH_RELATVE:
        {
            if (offset < 0)
            {
                if (pos < static_cast<size_t>(-offset))
                {
                    return VK_FALSE;
                }
            }
            else if (offset > 0)
            {
                if (size - pos <= static_cast<size_t>(offset))
                {
                    return VK_FALSE;
                }
            }

            pos += offset;

            return VK_TRUE;
        }
        break;
    }

    return VK_FALSE;
}

size_t BinaryBuffer::read(void* ptr, const size_t sizeElement,
                          const size_t countElement)
{
    if (!data)
    {
        return 0;
    }

    if (!ptr || sizeElement == 0 || countElement == 0)
    {
        return 0;
    }

    if (pos >= size)
    {
        return 0;
    }

    size_t bytesRead = sizeElement * countElement;
    bytesRead = glm::min(bytesRead, size - pos);

    size_t countElementRead = bytesRead / sizeElement;

    bytesRead = sizeElement * countElementRead;

    uint8_t* dataPtr = static_cast<uint8_t*>(ptr);

    for (size_t i = 0; i < bytesRead; i++)
    {
        dataPtr[i] = data[pos];

        pos++;
    }

    return countElementRead;
}

size_t BinaryBuffer::write(const void* ptr, const size_t sizeElement,
                           const size_t countElement)
{
    if (!data)
    {
        return 0;
    }

    if (!ptr || sizeElement == 0 || countElement == 0)
    {
        return 0;
    }

    size_t bytesWrite = sizeElement * countElement;

    if (pos >= size)
    {
        uint8_t* tempData = new uint8_t[size + bytesWrite];

        if (!tempData)
        {
            return 0;
        }

        memcpy(tempData, data, size);

        size += bytesWrite;

        delete[] data;

        data = tempData;
    }

    bytesWrite = glm::min(bytesWrite, size - pos);

    size_t countElementWrite = bytesWrite / sizeElement;

    bytesWrite = sizeElement * countElementWrite;

    const uint8_t* dataPtr = static_cast<const uint8_t*>(ptr);

    for (size_t i = 0; i < bytesWrite; i++)
    {
        data[pos] = dataPtr[i];

        pos++;
    }

    return countElementWrite;
}

VkBool32 BinaryBuffer::copy(void* data) const
{
    if (!data || !getData())
    {
        return VK_FALSE;
    }

    memcpy(data, getData(), getSize());

    return VK_TRUE;
}

//
// ICloneable
//

IBinaryBufferSP BinaryBuffer::clone() const
{
    return IBinaryBufferSP(new BinaryBuffer(data, size));
}

} /* namespace vkts */
