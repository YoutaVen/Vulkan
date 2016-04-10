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

#ifndef VKTS_IMAGEVIEW_HPP_
#define VKTS_IMAGEVIEW_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class ImageView: public IImageView
{

private:

    const VkDevice device;

    const VkImageViewCreateInfo imageViewCreateInfo;

    VkImageView imageView;

public:

    ImageView() = delete;
    ImageView(const VkDevice device, const VkImageViewCreateFlags flags, const VkImage image, const VkImageViewType viewType, const VkFormat format, const VkComponentMapping& components, const VkImageSubresourceRange& subresourceRange, const VkImageView imageView);
    ImageView(const ImageView& other) = delete;
    ImageView(ImageView&& other) = delete;
    virtual ~ImageView();

    ImageView& operator =(const ImageView& other) = delete;

    ImageView& operator =(ImageView && other) = delete;

    //
    // IImageView
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkImageViewCreateInfo& getImageViewCreateInfo() const override;

    virtual VkImageViewCreateFlags getFlags() const override;

    virtual const VkImage getImage() const override;

    virtual VkImageViewType getViewType() const override;

    virtual VkFormat getFormat() const override;

    virtual const VkComponentMapping& getComponentMapping() const override;

    virtual VkComponentSwizzle getR() const override;

    virtual VkComponentSwizzle getG() const override;

    virtual VkComponentSwizzle getB() const override;

    virtual VkComponentSwizzle getA() const override;

    virtual const VkImageSubresourceRange& getSubresourceRange() const override;

    virtual VkImageAspectFlags getAspectMask() const override;

    virtual uint32_t getBaseMipLevel() const override;

    virtual uint32_t getLevelCount() const override;

    virtual uint32_t getBaseArrayLayer() const override;

    virtual uint32_t getLayerCount() const override;

    virtual const VkImageView getImageView() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_IMAGEVIEW_HPP_ */
