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

namespace vkts
{

SmartPointerVector<IImageDataSP> VKTS_APIENTRY imageDataMipmap(const IImageDataSP& sourceImage, VkBool32 addSourceAsCopy, const std::string& name)
{
    if (name.size() == 0 || !sourceImage.get())
    {
        return SmartPointerVector<IImageDataSP>();
    }

    std::string sourceImageFilename = name;

    auto dotIndex = sourceImageFilename.rfind(".");

    if (dotIndex == sourceImageFilename.npos)
    {
        return SmartPointerVector<IImageDataSP>();
    }

    auto sourceImageName = sourceImageFilename.substr(0, dotIndex);
    auto sourceImageExtension = sourceImageFilename.substr(dotIndex);

    IImageDataSP currentSourceImage = sourceImage;
    int32_t width = currentSourceImage->getWidth();
    int32_t height = currentSourceImage->getHeight();
    int32_t depth = currentSourceImage->getDepth();

    IImageDataSP currentTargetImage;
    std::string targetImageFilename;

    int32_t level = 0;

    SmartPointerVector<IImageDataSP> result;

    if (addSourceAsCopy)
    {
        targetImageFilename = sourceImageName + "_L" + std::to_string(level++) + sourceImageExtension;

        currentTargetImage = imageDataCopy(sourceImage, targetImageFilename);

        if (!currentTargetImage.get())
        {
            return SmartPointerVector<IImageDataSP>();
        }

        result.append(currentTargetImage);
    }
    else
    {
        result.append(sourceImage);

        level++;
    }

    while (width > 1 || height > 1 || depth > 1)
    {
        width = glm::max(width / 2, 1);
        height = glm::max(height / 2, 1);
        depth = glm::max(depth / 2, 1);

        targetImageFilename = sourceImageName + "_L" + std::to_string(level++) + sourceImageExtension;

        currentTargetImage = imageDataCreate(targetImageFilename, width, height, depth, 0.0f, 0.0f, 0.0f, 0.0f, sourceImage->getImageType(), sourceImage->getFormat());

        if (!currentTargetImage.get())
        {
            return SmartPointerVector<IImageDataSP>();
        }

        for (int32_t z = 0; z < depth; z++)
        {
            for (int32_t y = 0; y < height; y++)
            {
                for (int32_t x = 0; x < width; x++)
                {
                    glm::vec4 rgba = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
                    float sampleCount = 0.0f;

                    for (int32_t iz = z * 2; iz < z * 2 + 1; iz++)
                    {
                        for (int32_t iy = y * 2; iy < y * 2 + 1; iy++)
                        {
                            for (int32_t ix = x * 2; ix < x * 2 + 1; ix++)
                            {
                                rgba += currentSourceImage->getTexel(ix, iy, iz);

                                sampleCount += 1.0f;
                            }
                        }
                    }

                    if (sampleCount > 0.0f)
                    {
                        rgba /= sampleCount;

                        currentTargetImage->setTexel(rgba, x, y, z);
                    }
                }
            }
        }

        result.append(currentTargetImage);

        //

        currentSourceImage = currentTargetImage;
        width = currentSourceImage->getWidth();
        height = currentSourceImage->getHeight();
        depth = currentSourceImage->getDepth();
    }

    return result;
}

}
