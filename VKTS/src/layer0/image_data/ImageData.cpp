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

#include "../binary_buffer/BinaryBuffer.hpp"
#include "ImageData.hpp"

namespace vkts
{

void ImageData::reset()
{
    name = "";

    imageType = VK_IMAGE_TYPE_1D;

    format = VK_FORMAT_UNDEFINED;

    extent = { 0, 0, 0};

    mipLevels = 0;

    if (buffer.get())
    {
        buffer->reset();
    }

    UNORM = VK_FALSE;
    SFLOAT = VK_FALSE;
    bytesPerChannel = 0;
    numberChannels = 0;
}

ImageData::ImageData(const std::string& name, const VkImageType imageType, const VkFormat& format, const VkExtent3D& extent, const uint32_t mipLevels, const uint8_t* data, const size_t size) :
    IImageData(), name(name), imageType(imageType), format(format), extent(extent), mipLevels(mipLevels)
{
    buffer = IBinaryBufferSP(new BinaryBuffer(data, size));

    if (!buffer.get() || !buffer->getData())
    {
        reset();
    }

    UNORM = commonIsUNORM(format);
    SFLOAT = commonIsSFLOAT(format);
    bytesPerChannel = commonGetBytesPerChannel(format);
    numberChannels = commonGetNumberChannels(format);
}

ImageData::ImageData(const std::string& name, const VkImageType imageType, const VkFormat& format, const VkExtent3D& extent, const uint32_t mipLevels, const IBinaryBufferSP& buffer) :
    IImageData(), name(name), imageType(imageType), format(format), extent(extent), mipLevels(mipLevels), buffer(buffer)
{
    if (!this->buffer.get() || !this->buffer->getData())
    {
        reset();
    }

    UNORM = commonIsUNORM(format);
    SFLOAT = commonIsSFLOAT(format);
    bytesPerChannel = commonGetBytesPerChannel(format);
    numberChannels = commonGetNumberChannels(format);
}

ImageData::~ImageData()
{
    reset();
}

//
// IImageData
//

const std::string& ImageData::getName() const
{
    return name;
}

VkImageType ImageData::getImageType() const
{
    return imageType;
}

const VkFormat& ImageData::getFormat() const
{
    return format;
}

const VkExtent3D& ImageData::getExtent3D() const
{
    return extent;
}

uint32_t ImageData::getWidth() const
{
    return extent.width;
}

uint32_t ImageData::getHeight() const
{
    return extent.height;
}

uint32_t ImageData::getDepth() const
{
    return extent.depth;
}

uint32_t ImageData::getMipLevels() const
{
    return mipLevels;
}

const void* ImageData::getData() const
{
    if (buffer.get())
    {
        return buffer->getData();
    }

    return nullptr;
}

size_t ImageData::getSize() const
{
    if (buffer.get())
    {
        return buffer->getSize();
    }

    return 0;
}

VkBool32 ImageData::copy(void* data, const uint32_t mipLevel,
                         const VkSubresourceLayout& subresourceLayout) const
{
    if (!data || mipLevel >= mipLevels || !getData())
    {
        return VK_FALSE;
    }

    if (bytesPerChannel == 0 || numberChannels == 0)
    {
        return VK_FALSE;
    }

    size_t offset = 0;

    VkExtent3D currentExtent = extent;

    for (uint32_t i = 0; i < mipLevel; i++)
    {
        offset += bytesPerChannel * numberChannels * currentExtent.width * currentExtent.height * currentExtent.depth;

        currentExtent.width = glm::max(extent.width >> (i + 1), 1u);
        currentExtent.height = glm::max(extent.height >> (i + 1), 1u);
        currentExtent.depth = glm::max(extent.depth >> (i + 1), 1u);
    }

    const uint8_t* currentSourceBuffer = &(static_cast<const uint8_t*>(buffer->getData())[offset]);

    uint8_t* currentTargetBuffer = static_cast<uint8_t*>(data);

    //

    const uint8_t* currentSourceChannel = nullptr;

    uint8_t* currentTargetChannel = nullptr;

    for (uint32_t z = 0; z < currentExtent.depth; z++)
    {
        for (uint32_t y = 0; y < currentExtent.height; y++)
        {
            currentSourceChannel = &currentSourceBuffer[y * currentExtent.width * numberChannels * bytesPerChannel + z * currentExtent.height * currentExtent.width * numberChannels * bytesPerChannel];

            currentTargetChannel = &currentTargetBuffer[y * subresourceLayout.rowPitch + z * subresourceLayout.depthPitch + subresourceLayout.offset];

            memcpy(currentTargetChannel, currentSourceChannel, numberChannels * bytesPerChannel * currentExtent.width);
        }
    }

    return VK_TRUE;
}

VkBool32 ImageData::upload(const void* data, const uint32_t mipLevel, const VkSubresourceLayout& subresourceLayout) const
{
    if (!data || mipLevel >= mipLevels || !getData())
    {
        return VK_FALSE;
    }

    if (bytesPerChannel == 0 || numberChannels == 0)
    {
        return VK_FALSE;
    }

    size_t offset = 0;

    VkExtent3D currentExtent = extent;

    for (uint32_t i = 0; i < mipLevel; i++)
    {
        offset += bytesPerChannel * numberChannels * currentExtent.width * currentExtent.height * currentExtent.depth;

        currentExtent.width = glm::max(extent.width >> (i + 1u), 1u);
        currentExtent.height = glm::max(extent.height >> (i + 1u), 1u);
        currentExtent.depth = glm::max(extent.depth >> (i + 1u), 1u);
    }

    if (subresourceLayout.size > (VkDeviceSize)(buffer->getSize() - offset))
    {
        return VK_FALSE;
    }

    const uint8_t* currentSourceBuffer = static_cast<const uint8_t*>(data);

    if (!buffer->seek(offset, VKTS_SEARCH_ABSOLUTE))
    {
        return VK_FALSE;
    }

    //

    const uint8_t* currentSourceChannel = nullptr;

    for (uint32_t z = 0; z < currentExtent.depth; z++)
    {
        for (uint32_t y = 0; y < currentExtent.height; y++)
        {
            currentSourceChannel = &currentSourceBuffer[y * subresourceLayout.rowPitch + z * subresourceLayout.depthPitch + subresourceLayout.offset];

            buffer->write(currentSourceChannel, 1, numberChannels * bytesPerChannel * currentExtent.width);
        }
    }

    return VK_TRUE;
}

VkBool32 ImageData::isUNORM() const
{
    return UNORM;
}

VkBool32 ImageData::isSFLOAT() const
{
    return SFLOAT;
}

int32_t ImageData::getBytesPerChannel() const
{
    return bytesPerChannel;
}

int32_t ImageData::getNumberChannels() const
{
    return numberChannels;
}

void ImageData::setTexel(const glm::vec4& rgba, const uint32_t x, const uint32_t y, const uint32_t z)
{
    if (x >= extent.width || y >= extent.height || z >= extent.depth)
    {
        return;;
    }

    buffer->seek(numberChannels * bytesPerChannel * x + numberChannels * bytesPerChannel * y * extent.width + numberChannels * bytesPerChannel * z * extent.width * extent.height, VKTS_SEARCH_ABSOLUTE);

    if (UNORM)
    {
        uint8_t texelStart[4];

        int32_t currentTargetChannelIndex;

        for (int32_t currentChannelIndex = 0; currentChannelIndex < numberChannels; currentChannelIndex++)
        {
            currentTargetChannelIndex = currentChannelIndex;

            if (format == VK_FORMAT_B8G8R8_UNORM || format == VK_FORMAT_B8G8R8A8_UNORM)
            {
                if (currentChannelIndex == 0)
                {
                    currentTargetChannelIndex = 2;
                }
                else if (currentChannelIndex == 2)
                {
                    currentTargetChannelIndex = 0;
                }
            }

            texelStart[currentTargetChannelIndex] = (uint8_t)(rgba[currentChannelIndex] * 255.0f);
        }

        buffer->write(texelStart, sizeof(uint8_t), numberChannels);
    }
    else if (SFLOAT)
    {
        float texelStart[4];

        for (int32_t currentChannelIndex = 0; currentChannelIndex < numberChannels; currentChannelIndex++)
        {
            texelStart[currentChannelIndex] = rgba[currentChannelIndex];
        }

        buffer->write(texelStart, sizeof(float), numberChannels);
    }
}

glm::vec4 ImageData::getTexel(const uint32_t x, const uint32_t y, const uint32_t z) const
{
    if (x >= extent.width || y >= extent.height || z >= extent.depth)
    {
        return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    buffer->seek(numberChannels * bytesPerChannel * x + numberChannels * bytesPerChannel * y * extent.width + numberChannels * bytesPerChannel * z * extent.width * extent.height, VKTS_SEARCH_ABSOLUTE);

    glm::vec4 result(0.0f, 0.0f, 0.0f, 1.0f);

    if (UNORM)
    {
        uint8_t texelStart[4];

        buffer->read(texelStart, sizeof(uint8_t), numberChannels);

        int32_t currentTargetChannelIndex;

        for (int32_t currentChannelIndex = 0; currentChannelIndex < numberChannels; currentChannelIndex++)
        {
            currentTargetChannelIndex = currentChannelIndex;

            if (format == VK_FORMAT_B8G8R8_UNORM || format == VK_FORMAT_B8G8R8A8_UNORM)
            {
                if (currentChannelIndex == 0)
                {
                    currentTargetChannelIndex = 2;
                }
                else if (currentChannelIndex == 2)
                {
                    currentTargetChannelIndex = 0;
                }
            }

            result[currentTargetChannelIndex] = (float)(texelStart[currentChannelIndex]) / 255.0f;
        }
    }
    else if (SFLOAT)
    {
        float texelStart[4];

        buffer->read(texelStart, sizeof(float), numberChannels);

        for (int32_t currentChannelIndex = 0; currentChannelIndex < numberChannels; currentChannelIndex++)
        {
            result[currentChannelIndex] = texelStart[currentChannelIndex];
        }
    }

    return result;
}

} /* namespace vkts */
