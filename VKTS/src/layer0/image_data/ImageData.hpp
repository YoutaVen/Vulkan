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

#ifndef VKTS_IMAGEDATA_HPP_
#define VKTS_IMAGEDATA_HPP_

#include <vkts/vkts.hpp>

#include "ImageData.hpp"

namespace vkts
{

class ImageData: public IImageData
{

private:

    std::string name;

    VkImageType imageType;
    VkFormat format;
    VkExtent3D extent;
    uint32_t mipLevels;

    IBinaryBufferSP buffer;

    VkBool32 UNORM;
    VkBool32 SFLOAT;
    int32_t bytesPerChannel;
    int32_t numberChannels;

    void
    reset();

public:

    ImageData() = delete;
    ImageData(const std::string& name, const VkImageType imageType, const VkFormat& format, const VkExtent3D& extent, const uint32_t mipLevels, const uint8_t* data, const size_t size);
    ImageData(const std::string& name, const VkImageType imageType, const VkFormat& format, const VkExtent3D& extent, const uint32_t mipLevels, const IBinaryBufferSP& buffer);
    ImageData(const ImageData& other) = delete;
    ImageData(ImageData&& other) = delete;
    virtual ~ImageData();

    ImageData& operator =(const ImageData& other) = delete;

    ImageData& operator =(ImageData && other) = delete;

    //
    // IImageData
    //

    virtual const std::string& getName() const override;

    virtual VkImageType getImageType() const override;

    virtual const VkFormat& getFormat() const override;

    virtual const VkExtent3D& getExtent3D() const override;

    virtual uint32_t getWidth() const override;

    virtual uint32_t getHeight() const override;

    virtual uint32_t getDepth() const override;

    virtual uint32_t getMipLevels() const override;

    virtual const void* getData() const override;

    virtual size_t getSize() const override;

    virtual VkBool32 copy(void* data, const uint32_t mipLevel, const VkSubresourceLayout& subresourceLayout) const override;

    virtual VkBool32 upload(const void* data, const uint32_t mipLevel, const VkSubresourceLayout& subresourceLayout) const override;

    virtual VkBool32 isUNORM() const override;

    virtual VkBool32 isSFLOAT() const override;

    virtual int32_t getBytesPerChannel() const override;

    virtual int32_t getNumberChannels() const override;

    virtual void setTexel(const glm::vec4& rgba, const uint32_t x, const uint32_t y, const uint32_t z) override;

    virtual glm::vec4 getTexel(const uint32_t x, const uint32_t y, const uint32_t z) const override;

};

} /* namespace vkts */

#endif /* VKTS_IMAGEDATA_HPP_ */
