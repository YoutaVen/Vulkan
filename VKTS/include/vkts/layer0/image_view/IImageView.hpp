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

#ifndef VKTS_IIMAGEVIEW_HPP_
#define VKTS_IIMAGEVIEW_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class IImageView: public IDestroyable
{

public:

    IImageView() :
        IDestroyable()
    {
    }

    virtual ~IImageView()
    {
    }

    virtual const VkDevice getDevice() const = 0;

    virtual const VkImageViewCreateInfo& getImageViewCreateInfo() const = 0;

    virtual VkImageViewCreateFlags getFlags() const = 0;

    virtual const VkImage getImage() const = 0;

    virtual VkImageViewType getViewType() const = 0;

    virtual VkFormat getFormat() const = 0;

    virtual const VkComponentMapping& getComponentMapping() const = 0;

    virtual VkComponentSwizzle getR() const = 0;

    virtual VkComponentSwizzle getG() const = 0;

    virtual VkComponentSwizzle getB() const = 0;

    virtual VkComponentSwizzle getA() const = 0;

    virtual const VkImageSubresourceRange& getSubresourceRange() const = 0;

    virtual VkImageAspectFlags getAspectMask() const = 0;

    virtual uint32_t getBaseMipLevel() const = 0;

    virtual uint32_t getLevelCount() const = 0;

    virtual uint32_t getBaseArrayLayer() const = 0;

    virtual uint32_t getLayerCount() const = 0;

    virtual const VkImageView getImageView() const = 0;

    //
    // IDestroyable
    //

    virtual void destroy() = 0;

};

typedef std::shared_ptr<IImageView> IImageViewSP;

} /* namespace vkts */

#endif /* VKTS_IIMAGEVIEW_HPP_ */
