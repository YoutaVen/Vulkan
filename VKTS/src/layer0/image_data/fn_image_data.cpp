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

#include "../binary_buffer/BinaryBuffer.hpp"

#include "ImageData.hpp"

#define VKTS_HDR_HEADER_SIZE 52

namespace vkts
{

template<typename T>
static void imageDataSwapRedBlueChannel(const size_t numberChannels, T* data, const size_t length)
{
    if (!(numberChannels == 3 || numberChannels == 4) || !data || length == 0)
    {
        return;
    }

    T copyChannel;

    for (size_t i = 0; i < length; i++)
    {
        copyChannel = data[i * numberChannels + 0];
        data[i * numberChannels + 0] = data[i * numberChannels + 2];
        data[i * numberChannels + 2] = copyChannel;
    }
}

template<typename T>
static void imageDataConvertRGBtoRGBA(const T alpha, T* targetData, const T* sourceData, const size_t length)
{
    if (!targetData || !sourceData || length == 0)
    {
        return;
    }

    for (size_t i = 0; i < length; i++)
    {
        for (size_t k = 0; k < 3; k++)
        {
            targetData[i * 4 + k] = sourceData[i * 3 + k];
        }

        targetData[i * 4 + 3] = alpha;
    }
}

static void imageDataConvertRGBEtoRGB(float* rgb, const uint8_t* rgbe)
{
    if (!rgb || !rgbe)
    {
        return;
    }

    float exponent = static_cast<float>(rgbe[3] - 128);

    float twoRaisedExponent = powf(2.0f, exponent);

    rgb[0] = static_cast<float>(rgbe[0]) / 256.0f * twoRaisedExponent;
    rgb[1] = static_cast<float>(rgbe[1]) / 256.0f * twoRaisedExponent;
    rgb[2] = static_cast<float>(rgbe[2]) / 256.0f * twoRaisedExponent;
}

static void imageDataConvertRGBtoRGBE(uint8_t* rgbe, const float* rgb)
{
    if (!rgbe || !rgb)
    {
        return;
    }

    float significant[3];
    int32_t exponent[3];

    significant[0] = frexpf(rgb[0], &exponent[0]);
    significant[1] = frexpf(rgb[1], &exponent[1]);
    significant[2] = frexpf(rgb[2], &exponent[2]);

    int32_t maxExponent = exponent[0];
    if (exponent[1] > maxExponent)
    {
        maxExponent = exponent[1];
    }
    if (exponent[2] > maxExponent)
    {
        maxExponent = exponent[2];
    }

    rgbe[0] = static_cast<uint8_t>(significant[0] * 256.0f * powf(2.0f, static_cast<float>(exponent[0] - maxExponent)));
    rgbe[1] = static_cast<uint8_t>(significant[1] * 256.0f * powf(2.0f, static_cast<float>(exponent[1] - maxExponent)));
    rgbe[2] = static_cast<uint8_t>(significant[2] * 256.0f * powf(2.0f, static_cast<float>(exponent[2] - maxExponent)));
    rgbe[3] = static_cast<uint8_t>(maxExponent + 128);
}

static int32_t imageDataDecodeNewRLE(const IBinaryBufferSP& buffer, uint8_t* scanline, int32_t width)
{
    if (!buffer.get() || !scanline || width < 1)
    {
        return -1;
    }

    int32_t maxScanLength = 0;

    // read each component
    for (int32_t channel = 0; channel < 4; channel++)
    {
        int32_t scanLength = 0;

        int32_t x = 0;

        while (x < width)
        {
            uint8_t code;

            if (buffer->read(&code, 1, 1) != 1)
            {
                return -1;
            }

            uint8_t channelValue;

            if (code > 128)
            {
                // Run

                code &= 127;

                scanLength += code;

                if (scanLength > width)
                {
                    return -1;
                }

                if (buffer->read(&channelValue, 1, 1) != 1)
                {
                    return -1;
                }

                while (code--)
                {
                    scanline[x++ * 4 + channel] = channelValue;
                }
            }
            else
            {
                // Non-run

                scanLength += code;

                if (scanLength > width)
                {
                    return -1;
                }

                while (code--)
                {
                    if (buffer->read(&channelValue, 1, 1) != 1)
                    {
                        return -1;
                    }

                    scanline[x++ * 4 + channel] = channelValue;
                }
            }
        }

        if (scanLength > maxScanLength)
        {
            maxScanLength = scanLength;
        }
    }

    return maxScanLength;
}

static IImageDataSP imageDataLoadTga(const std::string& name, const IBinaryBufferSP& buffer)
{
    if (!buffer.get())
    {
        return IImageDataSP();
    }

    if (!buffer->seek(2, VKTS_SEARCH_RELATVE))
    {
        return IImageDataSP();
    }

    // read the image type
    uint8_t imageType;

    if (buffer->read(&imageType, 1, 1) != 1)
    {
        return IImageDataSP();
    }

    // check the image type
    if (imageType != 1 && imageType != 2 && imageType != 3 && imageType != 9 && imageType != 10 && imageType != 11)
    {
        return IImageDataSP();
    }

    VkBool32 hasColorMap = VK_FALSE;
    if (imageType == 1 || imageType == 9)
    {
        hasColorMap = VK_TRUE;
    }

    uint16_t offsetIndexColorMap;
    uint16_t lengthColorMap;
    uint16_t bitsPerPixelColorMap;
    size_t numberChannelsColorMap = 0;

    if (hasColorMap)
    {
        if (buffer->read(&offsetIndexColorMap, 2, 1) != 1)
        {
            return IImageDataSP();
        }

        if (buffer->read(&lengthColorMap, 2, 1) != 1)
        {
            return IImageDataSP();
        }

        if (buffer->read(&bitsPerPixelColorMap, 1, 1) != 1)
        {
            return IImageDataSP();
        }

        numberChannelsColorMap = bitsPerPixelColorMap / 8;

        // seek through the tga header, up to the width/height:
        if (!buffer->seek(4, VKTS_SEARCH_RELATVE))
        {
            return IImageDataSP();
        }
    }
    else
    {
        // seek through the tga header, up to the width/height:
        if (!buffer->seek(9, VKTS_SEARCH_RELATVE))
        {
            return IImageDataSP();
        }
    }

    uint16_t width;
    uint16_t height;
    uint16_t depth = 1;

    // read the width
    if (buffer->read(&width, 2, 1) != 1)
    {
        return IImageDataSP();
    }

    // read the height
    if (buffer->read(&height, 2, 1) != 1)
    {
        return IImageDataSP();
    }

    uint8_t bitsPerPixel;

    // read the bits per pixel
    if (buffer->read(&bitsPerPixel, 1, 1) != 1)
    {
        return IImageDataSP();
    }

    if (bitsPerPixel != 8 && bitsPerPixel != 24 && bitsPerPixel != 32)
    {
        return IImageDataSP();
    }

    size_t numberChannels = bitsPerPixel / 8;

    // move file pointer to beginning of targa data
    if (!buffer->seek(1, VKTS_SEARCH_RELATVE))
    {
        return IImageDataSP();
    }

    std::shared_ptr<uint8_t> colorMap;

    if (hasColorMap)
    {
        // Create color map space.

        colorMap = std::shared_ptr<uint8_t>(new uint8_t[lengthColorMap * numberChannelsColorMap], [](uint8_t* ptr){delete[] ptr;});

        if (!colorMap.get())
        {
            return IImageDataSP();
        }

        // Read in the color map.
        if (buffer->read(colorMap.get(), 1, lengthColorMap * numberChannelsColorMap) != lengthColorMap * numberChannelsColorMap)
        {
            return IImageDataSP();
        }

        // swap the color if necessary
        if (bitsPerPixelColorMap == 24 || bitsPerPixelColorMap == 32)
        {
            imageDataSwapRedBlueChannel(numberChannelsColorMap, colorMap.get(), lengthColorMap);
        }
    }

    std::shared_ptr<uint8_t> data = std::shared_ptr<uint8_t>(new uint8_t[width * height * depth * numberChannels], [](uint8_t* ptr){delete[] ptr;});

    // verify memory allocation
    if (!data.get())
    {
        return IImageDataSP();
    }

    if (imageType == 1 || imageType == 2 || imageType == 3)
    {
        if (buffer->read(data.get(), 1, width * height * depth * numberChannels) != width * height * depth * numberChannels)
        {
            return IImageDataSP();
        }
    }
    else
    {
        // RLE encoded
        int32_t pixelsRead = 0;

        while (pixelsRead < width * height * depth)
        {
            uint8_t amount;

            if (buffer->read(&amount, 1, 1) != 1)
            {
                return IImageDataSP();
            }

            if (amount & 0x80)
            {
                amount &= 0x7F;

                amount++;

                if (buffer->read(&data.get()[pixelsRead * numberChannels], 1, numberChannels) != numberChannels)
                {
                    return IImageDataSP();
                }

                for (uint8_t i = 1; i < amount; i++)
                {
                    for (uint8_t k = 0; k < numberChannels; k++)
                    {
                        data.get()[(pixelsRead + i) * numberChannels + k] = data.get()[pixelsRead * numberChannels + k];
                    }
                }
            }
            else
            {
                amount &= 0x7F;

                amount++;

                if (buffer->read(&data.get()[pixelsRead * numberChannels], 1, amount * numberChannels) != amount * numberChannels)
                {
                    return IImageDataSP();
                }
            }

            pixelsRead += amount;
        }
    }

    // swap the color if necessary
    if (bitsPerPixel == 24 || bitsPerPixel == 32)
    {
        imageDataSwapRedBlueChannel(numberChannels, data.get(), width * height * depth);
    }

    if (hasColorMap)
    {
        // Allocating new memory, as current memory is a look up table index and not a color.

        std::shared_ptr<uint8_t> lookUp = data;

        data = std::shared_ptr<uint8_t>(new uint8_t[width * height * depth * numberChannelsColorMap], [](uint8_t* ptr){ delete[] ptr; });

        if (!data.get())
        {
            return IImageDataSP();
        }

        // Copy color values from the color map into the image data.

        for (int32_t i = 0; i < width * height * depth; i++)
        {
            for (int32_t k = 0; k < static_cast<int32_t>(numberChannelsColorMap); k++)
            {
                data.get()[i * numberChannelsColorMap + k] = colorMap.get()[(offsetIndexColorMap + lookUp.get()[i]) * numberChannelsColorMap + k];
            }
        }

        numberChannels = numberChannelsColorMap;
    }

    VkFormat format = VK_FORMAT_R8_UNORM;

    if (numberChannels == 3)
    {
        format = VK_FORMAT_R8G8B8_UNORM;
    }
    else if (numberChannels == 4)
    {
        format = VK_FORMAT_R8G8B8A8_UNORM;
    }

    return IImageDataSP(new ImageData(name, VK_IMAGE_TYPE_2D, format, { width, height, depth }, 1, data.get(), width * height * depth * numberChannels));
}

static IImageDataSP imageDataLoadHdr(const std::string& name, const IBinaryBufferSP& buffer)
{
    if (!buffer.get())
    {
        return IImageDataSP();
    }

    char tempBuffer[256];

    if (buffer->read(tempBuffer, 10, 1) != 1)
    {
        return IImageDataSP();
    }

    //
    // Information header
    //

    // Identifier
    if (strncmp(tempBuffer, "#?RADIANCE", 10))
    {
        return IImageDataSP();
    }

    // Go to variables
    if (!buffer->seek(1, VKTS_SEARCH_RELATVE))
    {
        return IImageDataSP();
    }

    // Variables
    char currentChar = 0;
    char oldChar = 0;
    while (VK_TRUE)
    {
        oldChar = currentChar;

        if (buffer->read(&currentChar, 1, 1) != 1)
        {
            return IImageDataSP();
        }

        // Empty line indicates end of header
        if (currentChar == '\n' && oldChar == '\n')
        {
            break;
        }
    }

    // Resolution
    int32_t charIndex = 0;
    while (VK_TRUE)
    {
        if (buffer->read(&currentChar, 1, 1) != 1)
        {
            return IImageDataSP();
        }

        tempBuffer[charIndex++] = currentChar;

        if (currentChar == '\n')
        {
            break;
        }
    }

    int32_t width;
    int32_t height;

    if (!sscanf(tempBuffer, "-Y %d +X %d", &height, &width))
    {
        return IImageDataSP();
    }

    int32_t depth = 1;
    size_t numberChannels = 3;

    std::unique_ptr<float[]> data = std::unique_ptr<float[]>(new float[width * height * depth * numberChannels]);

    if (!data.get())
    {
        return IImageDataSP();
    }

    // Scanlines
    std::unique_ptr<uint8_t[]> scanline = std::unique_ptr<uint8_t[]>(new uint8_t[width * sizeof(float)]);

    if (!scanline.get())
    {
        return IImageDataSP();
    }

    float rgb[3];
    uint8_t rgbe[4];
    uint8_t prevRgbe[4] = { 0, 0, 0, 0 };

    int32_t factor = 1;
    int32_t x = 0;
    int32_t y = height - 1;
    while (y >= 0)
    {
        if (buffer->read(tempBuffer, 4, 1) != 1)
        {
            return IImageDataSP();
        }

        int32_t repeat = 0;

        // Examine value
        if (width < 32768 && tempBuffer[0] == 2 && tempBuffer[1] == 2 && tempBuffer[2] == ((width >> 8) & 0xFF) && tempBuffer[3] == (width & 0xFF))
        {
            // New RLE decoding

            int32_t scanlinePixels = imageDataDecodeNewRLE(buffer, &scanline[0], width);

            if (scanlinePixels < 0)
            {
                return IImageDataSP();
            }

            for (int32_t i = 0; i < scanlinePixels; i++)
            {
                if (y < 0)
                {
                    return IImageDataSP();
                }

                imageDataConvertRGBEtoRGB(rgb, &scanline[i * 4]);

                data[(width * y + x) * 3 + 0] = rgb[0];
                data[(width * y + x) * 3 + 1] = rgb[1];
                data[(width * y + x) * 3 + 2] = rgb[2];

                x++;
                if (x >= width)
                {
                    y--;
                    x = 0;
                }
            }

            factor = 1;

            prevRgbe[0] = scanline[(scanlinePixels - 1) * 4 + 0];
            prevRgbe[1] = scanline[(scanlinePixels - 1) * 4 + 1];
            prevRgbe[2] = scanline[(scanlinePixels - 1) * 4 + 2];
            prevRgbe[3] = scanline[(scanlinePixels - 1) * 4 + 3];

            continue;
        }
        else if (tempBuffer[0] == 1 && tempBuffer[1] == 1 && tempBuffer[2] == 1)
        {
            // Old RLE decoding

            repeat = tempBuffer[3] * factor;

            rgbe[0] = prevRgbe[0];
            rgbe[1] = prevRgbe[1];
            rgbe[2] = prevRgbe[2];
            rgbe[3] = prevRgbe[3];

            factor *= 256;
        }
        else
        {
            // No RLE decoding

            repeat = 1;

            rgbe[0] = tempBuffer[0];
            rgbe[1] = tempBuffer[1];
            rgbe[2] = tempBuffer[2];
            rgbe[3] = tempBuffer[3];

            factor = 1;
        }

        imageDataConvertRGBEtoRGB(rgb, rgbe);

        while (repeat)
        {
            if (y < 0)
            {
                return IImageDataSP();
            }

            data[(width * y + x) * 3 + 0] = rgb[0];
            data[(width * y + x) * 3 + 1] = rgb[1];
            data[(width * y + x) * 3 + 2] = rgb[2];

            x++;
            if (x >= width)
            {
                y--;
                x = 0;
            }

            repeat--;
        }

        prevRgbe[0] = rgbe[0];
        prevRgbe[1] = rgbe[1];
        prevRgbe[2] = rgbe[2];
        prevRgbe[3] = rgbe[3];
    }

    return IImageDataSP(new ImageData(name, VK_IMAGE_TYPE_2D, VK_FORMAT_R32G32B32_SFLOAT, { (uint32_t)width, (uint32_t)height, (uint32_t)depth }, 1, reinterpret_cast<const uint8_t*>(&data[0]), width * height * depth * numberChannels * sizeof(float)));
}

IImageDataSP VKTS_APIENTRY imageDataLoad(const char* filename)
{
    if (!filename)
    {
        return IImageDataSP();
    }

    auto buffer = fileLoadBinary(filename);

    if (!buffer.get())
    {
        return IImageDataSP();
    }

    std::string lowerCaseFilename(filename);
    std::transform(lowerCaseFilename.begin(), lowerCaseFilename.end(), lowerCaseFilename.begin(), ::tolower);

    auto dotPos = lowerCaseFilename.rfind('.');
    if (dotPos == lowerCaseFilename.npos)
    {
        return IImageDataSP();
    }

    std::string lowerCaseExtension = lowerCaseFilename.substr(dotPos);

    if (lowerCaseExtension == ".tga")
    {
        return imageDataLoadTga(filename, buffer);
    }
    else if (lowerCaseExtension == ".hdr")
    {
        return imageDataLoadHdr(filename, buffer);
    }

    return IImageDataSP();
}

static IBinaryBufferSP imageDataSaveTga(const IImageDataSP& imageData)
{
    if (!imageData.get())
    {
        return IBinaryBufferSP();
    }

    if (!(imageData->getFormat() == VK_FORMAT_R8_UNORM || imageData->getFormat() == VK_FORMAT_R8G8B8_UNORM || imageData->getFormat() == VK_FORMAT_B8G8R8_UNORM || imageData->getFormat() == VK_FORMAT_R8G8B8A8_UNORM || imageData->getFormat() == VK_FORMAT_B8G8R8A8_UNORM))
    {
        return IBinaryBufferSP();
    }

    uint8_t bitsPerPixel;

    if (imageData->getFormat() == VK_FORMAT_R8_UNORM)
    {
        bitsPerPixel = 8;
    }
    else if (imageData->getFormat() == VK_FORMAT_R8G8B8_UNORM || imageData->getFormat() == VK_FORMAT_B8G8R8_UNORM)
    {
        bitsPerPixel = 24;
    }
    else if (imageData->getFormat() == VK_FORMAT_R8G8B8A8_UNORM || imageData->getFormat() == VK_FORMAT_B8G8R8A8_UNORM)
    {
        bitsPerPixel = 32;
    }
    else
    {
        return IBinaryBufferSP();
    }

    size_t numberChannels = bitsPerPixel / 8;

    // 18 bytes is the size of the header.
    IBinaryBufferSP buffer = IBinaryBufferSP(new BinaryBuffer(imageData->getWidth() * imageData->getHeight() * imageData->getDepth() * numberChannels + 18));

    if (!buffer.get())
    {
        return IBinaryBufferSP();
    }

    //

    uint8_t header[12];

    memset(header, 0, 12);

    if (bitsPerPixel == 8)
    {
        header[2] = 3;
    }
    else
    {
        header[2] = 2;
    }

    //

    if (buffer->write(header, 1, 12) != 12)
    {
        return IBinaryBufferSP();
    }

    uint16_t width = static_cast<uint16_t>(imageData->getWidth());
    uint16_t height = static_cast<uint16_t>(imageData->getHeight());
    uint16_t depth = 1;

    if (buffer->write(&width, 2, 1) != 1)
    {
        return IBinaryBufferSP();
    }

    if (buffer->write(&height, 2, 1) != 1)
    {
        return IBinaryBufferSP();
    }

    if (buffer->write(&bitsPerPixel, 1, 1) != 1)
    {
        return IBinaryBufferSP();
    }

    header[0] = 0;

    if (buffer->write(header, 1, 1) != 1)
    {
        return IBinaryBufferSP();
    }

    //

    std::unique_ptr<uint8_t[]> data = std::unique_ptr<uint8_t[]>(new uint8_t[width * height * depth * numberChannels]);

    if (!data.get())
    {
        return IBinaryBufferSP();
    }

    memcpy(&data[0], imageData->getData(), width * height * depth * numberChannels);

    if (imageData->getFormat() == VK_FORMAT_R8G8B8_UNORM || imageData->getFormat() == VK_FORMAT_R8G8B8A8_UNORM)
    {
        imageDataSwapRedBlueChannel(numberChannels, &data[0], width * height * depth);
    }

    if (buffer->write(&data[0], 1, width * height * depth * numberChannels) != width * height * depth * numberChannels)
    {
        return IBinaryBufferSP();
    }

    return buffer;
}

static IBinaryBufferSP imageDataSaveHdr(const IImageDataSP& imageData)
{
    if (!imageData.get())
    {
        return IBinaryBufferSP();
    }

    if (imageData->getFormat() != VK_FORMAT_R32G32B32_SFLOAT)
    {
        return IBinaryBufferSP();
    }

    char tempBuffer[256];

    if (snprintf(tempBuffer, 256, "-Y %d +X %d\n", imageData->getHeight(), imageData->getWidth()) < 0)
    {
        return IBinaryBufferSP();
    }

    size_t numberChannels = 3;

    // 52 bytes is the size of the header. RGB, where each channel is 4 bytes, is encoded in total of 4 bytes.
    IBinaryBufferSP buffer = IBinaryBufferSP(new BinaryBuffer((imageData->getWidth() * imageData->getHeight() * imageData->getDepth()) * 4 * sizeof(uint8_t) + VKTS_HDR_HEADER_SIZE + strlen(tempBuffer)));

    if (!buffer.get())
    {
        return IBinaryBufferSP();
    }

    // Header
    if (buffer->write("#?RADIANCE\n#Saved with VKTS\nFORMAT=32-bit_rle_rgbe\n\n", 1, VKTS_HDR_HEADER_SIZE) != VKTS_HDR_HEADER_SIZE)
    {
        return IBinaryBufferSP();
    }

    // Resolution
    if (buffer->write(tempBuffer, 1, strlen(tempBuffer)) != strlen(tempBuffer))
    {
        return IBinaryBufferSP();
    }

    uint8_t rgbe[4];

    const float* tempData = static_cast<const float*>(imageData->getData());

    // Non compressed data
    for (int32_t y = (int32_t)imageData->getHeight() - 1; y >= 0; y--)
    {
        for (int32_t x = 0; x < (int32_t)imageData->getWidth(); x++)
        {
            imageDataConvertRGBtoRGBE(rgbe, &tempData[(y * imageData->getWidth() + x) * numberChannels]);

            if (buffer->write(rgbe, 1, 4) != 4)
            {
                return IBinaryBufferSP();
            }
        }
    }

    return buffer;
}

VkBool32 VKTS_APIENTRY imageDataSave(const char* filename, const IImageDataSP& imageData)
{
    if (!filename || !imageData.get())
    {
        return VK_FALSE;
    }

    if (imageData->getImageType() != VK_IMAGE_TYPE_2D || imageData->getWidth() < 1 || imageData->getHeight() < 1 || imageData->getDepth() != 1)
    {
        return VK_FALSE;
    }

    std::string lowerCaseFilename(filename);
    std::transform(lowerCaseFilename.begin(), lowerCaseFilename.end(), lowerCaseFilename.begin(), ::tolower);

    auto dotPos = lowerCaseFilename.rfind('.');
    if (dotPos == lowerCaseFilename.npos)
    {
        return VK_FALSE;
    }

    std::string lowerCaseExtension = lowerCaseFilename.substr(dotPos);

    if (lowerCaseExtension == ".tga")
    {
        auto buffer = imageDataSaveTga(imageData);

        if (!buffer.get())
        {
            return VK_FALSE;
        }

        return fileSaveBinary(filename, buffer);
    }
    else if (lowerCaseExtension == ".hdr")
    {
        auto buffer = imageDataSaveHdr(imageData);

        if (!buffer.get())
        {
            return VK_FALSE;
        }

        return fileSaveBinary(filename, buffer);
    }

    return VK_FALSE;
}

IImageDataSP VKTS_APIENTRY imageDataCreate(const std::string& name, const uint32_t width, const uint32_t height, const uint32_t depth, const VkImageType imageType, const VkFormat& format)
{
    if (width < 1 || height < 1 || depth < 1)
    {
        return IImageDataSP();
    }

    if (imageType == VK_IMAGE_TYPE_1D && height != 1 && depth != 1)
    {
        return IImageDataSP();
    }
    else if (imageType == VK_IMAGE_TYPE_2D && depth != 1)
    {
        return IImageDataSP();
    }

    uint32_t bytesPerChannel;
    uint32_t numberChannels;

    std::unique_ptr<uint8_t[]> data;

    if (format == VK_FORMAT_R8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 1;
    }
    else if (format == VK_FORMAT_R8G8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 2;
    }
    else if (format == VK_FORMAT_R8G8B8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 3;
    }
    else if (format == VK_FORMAT_B8G8R8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 3;
    }
    else if (format == VK_FORMAT_R8G8B8A8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 4;
    }
    else if (format == VK_FORMAT_B8G8R8A8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 4;
    }
    else if (format == VK_FORMAT_R32_SFLOAT)
    {
        bytesPerChannel = 4;

        numberChannels = 1;
    }
    else if (format == VK_FORMAT_R32G32_SFLOAT)
    {
        bytesPerChannel = 4;

        numberChannels = 2;
    }
    else if (format == VK_FORMAT_R32G32B32_SFLOAT)
    {
        bytesPerChannel = 4;

        numberChannels = 3;
    }
    else if (format == VK_FORMAT_R32G32B32A32_SFLOAT)
    {
        bytesPerChannel = 4;

        numberChannels = 4;
    }
    else
    {
        return IImageDataSP();
    }

    data = std::unique_ptr<uint8_t[]>(new uint8_t[width * height * depth * numberChannels * bytesPerChannel]);

    if (!data.get())
    {
        return IImageDataSP();
    }

    memset(&data[0], 0, (width * height * depth * numberChannels * bytesPerChannel));

    return IImageDataSP(new ImageData(name, imageType, format, { width, height, depth }, 1, &data[0], width * height * depth * numberChannels * bytesPerChannel));
}

IImageDataSP VKTS_APIENTRY imageDataCreate(const std::string& name, const uint32_t width, const uint32_t height, const uint32_t depth, const float red, const float green, const float blue, const float alpha, const VkImageType imageType, const VkFormat& format)
{
    if (width < 1 || height < 1 || depth < 1)
    {
        return IImageDataSP();
    }

    if (imageType == VK_IMAGE_TYPE_1D && height != 1 && depth != 1)
    {
        return IImageDataSP();
    }
    else if (imageType == VK_IMAGE_TYPE_2D && depth != 1)
    {
        return IImageDataSP();
    }

    int32_t bytesPerChannel;
    uint32_t numberChannels;

    std::unique_ptr<uint8_t[]> data;

    uint8_t rgbaUINT8[4];
    float rgbaFLOAT[4];

    VkBool32 isUNORM = VK_FALSE;
    VkBool32 isSFLOAT = VK_FALSE;

    if (format == VK_FORMAT_R8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 1;

        rgbaUINT8[0] = static_cast<uint8_t>(glm::clamp(red, 0.0f, 1.0f) * 255.0f);

        isUNORM = VK_TRUE;
    }
    else if (format == VK_FORMAT_R8G8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 2;

        rgbaUINT8[0] = static_cast<uint8_t>(glm::clamp(red, 0.0f, 1.0f) * 255.0f);

        rgbaUINT8[1] = static_cast<uint8_t>(glm::clamp(green, 0.0f, 1.0f) * 255.0f);

        isUNORM = VK_TRUE;
    }
    else if (format == VK_FORMAT_R8G8B8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 3;

        rgbaUINT8[0] = static_cast<uint8_t>(glm::clamp(red, 0.0f, 1.0f) * 255.0f);

        rgbaUINT8[1] = static_cast<uint8_t>(glm::clamp(green, 0.0f, 1.0f) * 255.0f);
        rgbaUINT8[2] = static_cast<uint8_t>(glm::clamp(blue, 0.0f, 1.0f) * 255.0f);

        isUNORM = VK_TRUE;
    }
    else if (format == VK_FORMAT_B8G8R8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 3;

        rgbaUINT8[2] = static_cast<uint8_t>(glm::clamp(red, 0.0f, 1.0f) * 255.0f);

        rgbaUINT8[1] = static_cast<uint8_t>(glm::clamp(green, 0.0f, 1.0f) * 255.0f);

        rgbaUINT8[0] = static_cast<uint8_t>(glm::clamp(blue, 0.0f, 1.0f) * 255.0f);

        isUNORM = VK_TRUE;
    }
    else if (format == VK_FORMAT_R8G8B8A8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 4;

        rgbaUINT8[0] = static_cast<uint8_t>(glm::clamp(red, 0.0f, 1.0f) * 255.0f);

        rgbaUINT8[1] = static_cast<uint8_t>(glm::clamp(green, 0.0f, 1.0f) * 255.0f);

        rgbaUINT8[2] = static_cast<uint8_t>(glm::clamp(blue, 0.0f, 1.0f) * 255.0f);

        rgbaUINT8[3] = static_cast<uint8_t>(glm::clamp(alpha, 0.0f, 1.0f) * 255.0f);

        isUNORM = VK_TRUE;
    }
    else if (format == VK_FORMAT_B8G8R8A8_UNORM)
    {
        bytesPerChannel = 1;

        numberChannels = 4;

        rgbaUINT8[2] = static_cast<uint8_t>(glm::clamp(red, 0.0f, 1.0f) * 255.0f);

        rgbaUINT8[1] = static_cast<uint8_t>(glm::clamp(green, 0.0f, 1.0f) * 255.0f);

        rgbaUINT8[0] = static_cast<uint8_t>(glm::clamp(blue, 0.0f, 1.0f) * 255.0f);

        rgbaUINT8[3] = static_cast<uint8_t>(glm::clamp(alpha, 0.0f, 1.0f) * 255.0f);

        isUNORM = VK_TRUE;
    }
    else if (format == VK_FORMAT_R32_SFLOAT)
    {
        bytesPerChannel = 4;

        numberChannels = 1;

        rgbaFLOAT[0] = red;

        isSFLOAT = VK_TRUE;
    }
    else if (format == VK_FORMAT_R32G32_SFLOAT)
    {
        bytesPerChannel = 4;

        numberChannels = 2;

        rgbaFLOAT[0] = red;

        rgbaFLOAT[1] = green;

        isSFLOAT = VK_TRUE;
    }
    else if (format == VK_FORMAT_R32G32B32_SFLOAT)
    {
        bytesPerChannel = 4;

        numberChannels = 3;

        rgbaFLOAT[0] = red;

        rgbaFLOAT[1] = green;

        rgbaFLOAT[2] = blue;

        isSFLOAT = VK_TRUE;
    }
    else if (format == VK_FORMAT_R32G32B32A32_SFLOAT)
    {
        bytesPerChannel = 4;

        numberChannels = 4;

        rgbaFLOAT[0] = red;

        rgbaFLOAT[1] = green;

        rgbaFLOAT[2] = blue;

        rgbaFLOAT[3] = alpha;

        isSFLOAT = VK_TRUE;
    }
    else
    {
        return IImageDataSP();
    }

    data = std::unique_ptr<uint8_t[]>(new uint8_t[width * height * depth * numberChannels * bytesPerChannel]);

    if (!data.get())
    {
        return IImageDataSP();
    }

    uint8_t* currentChannel = nullptr;
    float* currentChannelFLOAT = nullptr;

    for (uint32_t z = 0; z < depth; z++)
    {
        for (uint32_t y = 0; y < height; y++)
        {
            for (uint32_t x = 0; x < width; x++)
            {
                for (uint32_t channel = 0; channel < numberChannels; channel++)
                {
                    currentChannel = &data[channel * bytesPerChannel + x * numberChannels * bytesPerChannel + y * width * numberChannels * bytesPerChannel + z * height * width * numberChannels * bytesPerChannel];

                    if (isUNORM)
                    {
                        *currentChannel = rgbaUINT8[channel];
                    }
                    else if (isSFLOAT)
                    {
                        currentChannelFLOAT = reinterpret_cast<float*>(currentChannel);

                        *currentChannelFLOAT = rgbaFLOAT[channel];
                    }
                }
            }
        }
    }

    return IImageDataSP(new ImageData(name, imageType, format, { width, height, depth }, 1, &data[0], width * height * depth * numberChannels * bytesPerChannel));
}

IImageDataSP VKTS_APIENTRY imageDataConvert(const IImageDataSP& sourceImage, const VkFormat targetFormat, const std::string& name)
{
    if (!sourceImage.get() || sourceImage->getMipLevels() != 1)
    {
        return IImageDataSP();
    }

    int32_t sourceNumberChannels;

    VkBool32 sourceIsUNORM = VK_TRUE;
    VkBool32 sourceIsSFLOAT = VK_FALSE;

    int8_t sourceRgbaIndices[4] =
    { 0, -1, -1, -1 };

    int32_t targetBytesPerChannel;
    int32_t targetNumberChannels;

    VkBool32 targetIsUNORM = VK_TRUE;
    VkBool32 targetIsSFLOAT = VK_FALSE;

    int8_t targetRgbaIndices[4] = { 0, -1, -1, -1 };

    uint8_t rgbaUINT8[4] = { 255, 255, 255, 255 };
    float rgbaFLOAT[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    switch (sourceImage->getFormat())
    {
        case VK_FORMAT_R8_UNORM:

            sourceNumberChannels = 1;

            break;

        case VK_FORMAT_R8G8_UNORM:

            sourceNumberChannels = 2;

            sourceRgbaIndices[1] = 1;

            break;

        case VK_FORMAT_R8G8B8_UNORM:

            sourceNumberChannels = 3;

            sourceRgbaIndices[1] = 1;
            sourceRgbaIndices[2] = 2;

            break;

        case VK_FORMAT_B8G8R8_UNORM:

            sourceNumberChannels = 3;

            sourceRgbaIndices[0] = 2;
            sourceRgbaIndices[1] = 1;
            sourceRgbaIndices[2] = 0;

            break;

        case VK_FORMAT_R8G8B8A8_UNORM:

            sourceNumberChannels = 4;

            sourceRgbaIndices[1] = 1;
            sourceRgbaIndices[2] = 2;
            sourceRgbaIndices[3] = 3;

            break;

        case VK_FORMAT_B8G8R8A8_UNORM:

            sourceNumberChannels = 4;

            sourceRgbaIndices[0] = 2;
            sourceRgbaIndices[1] = 1;
            sourceRgbaIndices[2] = 0;
            sourceRgbaIndices[3] = 3;

            break;

        case VK_FORMAT_R32_SFLOAT:

            sourceNumberChannels = 1;

            sourceIsUNORM = VK_FALSE;
            sourceIsSFLOAT = VK_TRUE;

            break;

        case VK_FORMAT_R32G32_SFLOAT:

            sourceNumberChannels = 2;

            sourceRgbaIndices[1] = 1;

            sourceIsUNORM = VK_FALSE;
            sourceIsSFLOAT = VK_TRUE;

            break;

        case VK_FORMAT_R32G32B32_SFLOAT:

            sourceNumberChannels = 3;

            sourceRgbaIndices[1] = 1;
            sourceRgbaIndices[2] = 2;

            sourceIsUNORM = VK_FALSE;
            sourceIsSFLOAT = VK_TRUE;

            break;

        case VK_FORMAT_R32G32B32A32_SFLOAT:

            sourceNumberChannels = 4;

            sourceRgbaIndices[1] = 1;
            sourceRgbaIndices[2] = 2;
            sourceRgbaIndices[3] = 3;

            sourceIsUNORM = VK_FALSE;
            sourceIsSFLOAT = VK_TRUE;

            break;

        default:
            return IImageDataSP();
    }

    //

    switch (targetFormat)
    {
        case VK_FORMAT_R8_UNORM:

            targetBytesPerChannel = 1;

            targetNumberChannels = 1;

            break;

        case VK_FORMAT_R8G8_UNORM:

            targetBytesPerChannel = 1;

            targetNumberChannels = 2;

            targetRgbaIndices[1] = 1;

            break;

        case VK_FORMAT_R8G8B8_UNORM:

            targetBytesPerChannel = 1;

            targetNumberChannels = 3;

            targetRgbaIndices[1] = 1;
            targetRgbaIndices[2] = 2;

            break;

        case VK_FORMAT_B8G8R8_UNORM:

            targetBytesPerChannel = 1;

            targetNumberChannels = 3;

            targetRgbaIndices[0] = 2;
            targetRgbaIndices[1] = 1;
            targetRgbaIndices[2] = 0;

            break;

        case VK_FORMAT_R8G8B8A8_UNORM:

            targetBytesPerChannel = 1;

            targetNumberChannels = 4;

            targetRgbaIndices[1] = 1;
            targetRgbaIndices[2] = 2;
            targetRgbaIndices[3] = 3;

            break;

        case VK_FORMAT_B8G8R8A8_UNORM:

            targetBytesPerChannel = 1;

            targetNumberChannels = 4;

            targetRgbaIndices[0] = 2;
            targetRgbaIndices[1] = 1;
            targetRgbaIndices[2] = 0;
            targetRgbaIndices[3] = 3;

            break;

        case VK_FORMAT_R32_SFLOAT:

            targetBytesPerChannel = 4;

            targetNumberChannels = 1;

            targetIsUNORM = VK_FALSE;
            targetIsSFLOAT = VK_TRUE;

            break;

        case VK_FORMAT_R32G32_SFLOAT:

            targetBytesPerChannel = 4;

            targetNumberChannels = 2;

            targetRgbaIndices[1] = 1;

            targetIsUNORM = VK_FALSE;
            targetIsSFLOAT = VK_TRUE;

            break;

        case VK_FORMAT_R32G32B32_SFLOAT:

            targetBytesPerChannel = 4;

            targetNumberChannels = 3;

            targetRgbaIndices[1] = 1;
            targetRgbaIndices[2] = 2;

            targetIsUNORM = VK_FALSE;
            targetIsSFLOAT = VK_TRUE;

            break;

        case VK_FORMAT_R32G32B32A32_SFLOAT:

            targetBytesPerChannel = 4;

            targetNumberChannels = 4;

            targetRgbaIndices[1] = 1;
            targetRgbaIndices[2] = 2;
            targetRgbaIndices[3] = 3;

            targetIsUNORM = VK_FALSE;
            targetIsSFLOAT = VK_TRUE;

            break;

        default:
            return IImageDataSP();
    }

    //

    size_t targetDataSize = sourceImage->getWidth() * sourceImage->getHeight() * sourceImage->getDepth() * targetNumberChannels * targetBytesPerChannel;

    std::unique_ptr<uint8_t[]> targetData = std::unique_ptr<uint8_t[]>(new uint8_t[targetDataSize]);

    if (!targetData.get())
    {
        return IImageDataSP();
    }

    const uint8_t* currentSourceUINT8 = (const uint8_t*) sourceImage->getData();
    const float* currentSourceFLOAT = (const float*) sourceImage->getData();

    uint8_t* currentTargetUINT8 = &targetData[0];
    float* currentTargetFLOAT = (float*) &targetData[0];

    for (int32_t z = 0; z < (int32_t)sourceImage->getDepth(); z++)
    {
        for (int32_t y = 0; y < (int32_t)sourceImage->getHeight(); y++)
        {
            for (int32_t x = 0; x < (int32_t)sourceImage->getWidth(); x++)
            {
                for (int32_t channel = 0; channel < targetNumberChannels;
                        channel++)
                {
                    if (channel < sourceNumberChannels)
                    {
                        if (sourceIsUNORM)
                        {
                            rgbaUINT8[sourceRgbaIndices[channel]] = currentSourceUINT8[channel + x * sourceNumberChannels + y * sourceImage->getWidth() * sourceNumberChannels + z * sourceImage->getHeight() * sourceImage->getWidth() * sourceNumberChannels];
                        }
                        else if (sourceIsSFLOAT)
                        {
                            rgbaFLOAT[sourceRgbaIndices[channel]] = currentSourceFLOAT[channel + x * sourceNumberChannels + y * sourceImage->getWidth() * sourceNumberChannels + z * sourceImage->getHeight() * sourceImage->getWidth() * sourceNumberChannels];
                        }
                    }

                    if (targetIsUNORM)
                    {
                        if (sourceIsUNORM)
                        {
                            currentTargetUINT8[targetRgbaIndices[channel] + x * targetNumberChannels + y * sourceImage->getWidth() * targetNumberChannels + z * sourceImage->getHeight() * sourceImage->getWidth() * targetNumberChannels] = rgbaUINT8[channel];
                        }
                        else if (sourceIsSFLOAT)
                        {
                            currentTargetUINT8[targetRgbaIndices[channel] + x * targetNumberChannels + y * sourceImage->getWidth() * targetNumberChannels + z * sourceImage->getHeight() * sourceImage->getWidth() * targetNumberChannels] = static_cast<uint8_t>(glm::clamp(rgbaFLOAT[channel], 0.0f, 1.0f) * 255.0f);
                        }
                    }
                    else if (targetIsSFLOAT)
                    {
                        if (sourceIsUNORM)
                        {
                            currentTargetFLOAT[targetRgbaIndices[channel] + x * targetNumberChannels + y * sourceImage->getWidth() * targetNumberChannels + z * sourceImage->getHeight() * sourceImage->getWidth() * targetNumberChannels] = static_cast<float>(rgbaUINT8[channel]) / 255.0f;
                        }
                        else if (sourceIsSFLOAT)
                        {
                            currentTargetFLOAT[targetRgbaIndices[channel] + x * targetNumberChannels + y * sourceImage->getWidth() * targetNumberChannels + z * sourceImage->getHeight() * sourceImage->getWidth() * targetNumberChannels] = rgbaFLOAT[channel];
                        }
                    }
                }
            }
        }
    }

    return IImageDataSP(new ImageData(name, sourceImage->getImageType(), targetFormat, sourceImage->getExtent3D(), 1, &targetData[0], sourceImage->getWidth() * sourceImage->getHeight() * sourceImage->getDepth() * targetNumberChannels * targetBytesPerChannel));
}

IImageDataSP VKTS_APIENTRY imageDataCopy(const IImageDataSP& sourceImage, const std::string& name)
{
    if (!sourceImage.get())
    {
        return IImageDataSP();
    }

    return IImageDataSP(new ImageData(name, sourceImage->getImageType(), sourceImage->getFormat(), sourceImage->getExtent3D(), sourceImage->getMipLevels(), (const uint8_t*) sourceImage->getData(), sourceImage->getSize()));
}

IImageDataSP VKTS_APIENTRY imageDataMerge(const SmartPointerVector<IImageDataSP>& sourceImages, const std::string& name)
{
    if (sourceImages.size() == 0)
    {
        return IImageDataSP();
    }

    size_t totalSize = 0;

    VkImageType imageType;
    VkFormat format;
    VkExtent3D extent;

    for (size_t i = 0; i < sourceImages.size(); i++)
    {
        totalSize += sourceImages[i]->getSize();

        if (i == 0)
        {
            imageType = sourceImages[i]->getImageType();
            format = sourceImages[i]->getFormat();
            extent = sourceImages[i]->getExtent3D();
        }
        else
        {
            if (imageType != sourceImages[i]->getImageType())
            {
                return IImageDataSP();
            }
            else if (format != sourceImages[i]->getFormat())
            {
                return IImageDataSP();
            }
        }
    }

    auto mergedImageData = IBinaryBufferSP(new BinaryBuffer(totalSize));

    if (!mergedImageData.get())
    {
        return IImageDataSP();
    }

    size_t offset = 0;

    for (size_t i = 0; i < sourceImages.size(); i++)
    {
        mergedImageData->write(sourceImages[i]->getData(), sizeof(uint8_t), sourceImages[i]->getSize());

        offset += sourceImages[i]->getSize();
    }

    return IImageDataSP(new ImageData(name, imageType, format, extent, (uint32_t) sourceImages.size(), mergedImageData));
}

}
