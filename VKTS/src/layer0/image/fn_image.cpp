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

#include "Image.hpp"

namespace vkts
{

IImageSP VKTS_APIENTRY imageCreate(const VkDevice device, const VkImageCreateFlags flags, const VkImageType imageType, const VkFormat format, const VkExtent3D& extent, const uint32_t mipLevels, const uint32_t arrayLayers, const VkSampleCountFlagBits samples, const VkImageTiling tiling, const VkImageUsageFlags usage, const VkSharingMode sharingMode, const uint32_t queueFamilyIndexCount, const uint32_t* queueFamilyIndices, const VkImageLayout initialLayout, const VkAccessFlags accessMask)
{
    if (!device)
    {
        return IImageSP();
    }

    VkResult result;

    VkImageCreateInfo imageCreateInfo;

    memset(&imageCreateInfo, 0, sizeof(VkImageCreateInfo));

    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;

    imageCreateInfo.flags = flags;
    imageCreateInfo.imageType = imageType;
    imageCreateInfo.format = format;
    imageCreateInfo.extent = extent;
    imageCreateInfo.mipLevels = mipLevels;
    imageCreateInfo.arrayLayers = arrayLayers;
    imageCreateInfo.samples = samples;
    imageCreateInfo.tiling = tiling;
    imageCreateInfo.usage = usage;
    imageCreateInfo.sharingMode = sharingMode;
    imageCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
    imageCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    imageCreateInfo.initialLayout = initialLayout;

    VkImage image;

    result = vkCreateImage(device, &imageCreateInfo, nullptr, &image);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Image: Could not create image.");

        return IImageSP();
    }

    auto newInstance = new Image(device, flags, imageType, format, extent, mipLevels, arrayLayers, samples, tiling, usage, sharingMode, queueFamilyIndexCount, queueFamilyIndices, initialLayout, accessMask, image);

    if (!newInstance)
    {
        vkDestroyImage(device, image, nullptr);

        return IImageSP();
    }

    return IImageSP(newInstance);
}

}
