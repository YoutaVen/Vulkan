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

#include "ImageView.hpp"

namespace vkts
{

IImageViewSP VKTS_APIENTRY imageViewCreate(const VkDevice device, const VkImageViewCreateFlags flags, const VkImage image, const VkImageViewType viewType, const VkFormat format, const VkComponentMapping& components, const VkImageSubresourceRange& subresourceRange)
{
    if (!device)
    {
        return IImageViewSP();
    }

    VkResult result;

    VkImageViewCreateInfo imageViewCreateInfo;

    memset(&imageViewCreateInfo, 0, sizeof(VkImageViewCreateInfo));

    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

    imageViewCreateInfo.flags = flags;
    imageViewCreateInfo.image = image;
    imageViewCreateInfo.viewType = viewType;
    imageViewCreateInfo.format = format;
    imageViewCreateInfo.components = components;
    imageViewCreateInfo.subresourceRange = subresourceRange;

    VkImageView imageView;

    result = vkCreateImageView(device, &imageViewCreateInfo, nullptr, &imageView);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "ImageView: Could not create image view.");

        return IImageViewSP();
    }

    auto newInstance = new ImageView(device, flags, image, viewType, format, components, subresourceRange, imageView);

    if (!newInstance)
    {
        vkDestroyImageView(device, imageView, nullptr);

        return IImageViewSP();
    }

    return IImageViewSP(newInstance);
}

}
