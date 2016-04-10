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

#include "ImageView.hpp"

namespace vkts
{

ImageView::ImageView(const VkDevice device, const VkImageViewCreateFlags flags, const VkImage image, const VkImageViewType viewType, const VkFormat format, const VkComponentMapping& components, const VkImageSubresourceRange& subresourceRange, const VkImageView imageView) :
    IImageView(), device(device), imageViewCreateInfo{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, flags, image, viewType, format, components, subresourceRange}, imageView(imageView)
{
}

ImageView::~ImageView()
{
    destroy();
}

//
// IImageView
//

const VkDevice ImageView::getDevice() const
{
    return device;
}

const VkImageViewCreateInfo& ImageView::getImageViewCreateInfo() const
{
    return imageViewCreateInfo;
}

VkImageViewCreateFlags ImageView::getFlags() const
{
    return imageViewCreateInfo.flags;
}

const VkImage ImageView::getImage() const
{
    return imageViewCreateInfo.image;
}

VkImageViewType ImageView::getViewType() const
{
    return imageViewCreateInfo.viewType;
}

VkFormat ImageView::getFormat() const
{
    return imageViewCreateInfo.format;
}

const VkComponentMapping& ImageView::getComponentMapping() const
{
    return imageViewCreateInfo.components;
}

VkComponentSwizzle ImageView::getR() const
{
    return imageViewCreateInfo.components.r;
}

VkComponentSwizzle ImageView::getG() const
{
    return imageViewCreateInfo.components.g;
}

VkComponentSwizzle ImageView::getB() const
{
    return imageViewCreateInfo.components.b;
}

VkComponentSwizzle ImageView::getA() const
{
    return imageViewCreateInfo.components.a;
}

const VkImageSubresourceRange& ImageView::getSubresourceRange() const
{
    return imageViewCreateInfo.subresourceRange;
}

VkImageAspectFlags ImageView::getAspectMask() const
{
    return imageViewCreateInfo.subresourceRange.aspectMask;
}

uint32_t ImageView::getBaseMipLevel() const
{
    return imageViewCreateInfo.subresourceRange.baseMipLevel;
}

uint32_t ImageView::getLevelCount() const
{
    return imageViewCreateInfo.subresourceRange.levelCount;
}

uint32_t ImageView::getBaseArrayLayer() const
{
    return imageViewCreateInfo.subresourceRange.baseArrayLayer;
}

uint32_t ImageView::getLayerCount() const
{
    return imageViewCreateInfo.subresourceRange.layerCount;
}

const VkImageView ImageView::getImageView() const
{
    return imageView;
}

//
// IDestroyable
//

void ImageView::destroy()
{
    if (imageView)
    {
        vkDestroyImageView(device, imageView, nullptr);

        imageView = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
