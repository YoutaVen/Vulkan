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

#ifndef VKTS_TEXTURE_HPP_
#define VKTS_TEXTURE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Texture: public ITexture
{

private:

    const IInitialResourcesSP initialResources;

    std::string name;

    IMemoryImageSP memoryImage;

    ISamplerSP sampler;

    IImageViewSP imageView;

public:

    Texture() = delete;
    Texture(const IInitialResourcesSP& initialResources, const std::string& name, const IMemoryImageSP& memoryImage, const ISamplerSP& sampler, const IImageViewSP& imageView);
    Texture(const Texture& other) = delete;
    Texture(Texture&& other) = delete;
    virtual ~Texture();

    Texture& operator =(const Texture& other) = delete;
    Texture& operator =(Texture && other) = delete;

    //
    // ITexture
    //

    virtual const IInitialResourcesSP& getInitialResources() const override;

    virtual const std::string& getName() const override;

    virtual const IMemoryImageSP& getMemoryImage() const override;

    virtual const ISamplerSP& getSampler() const override;

    virtual const IImageViewSP& getImageView() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_TEXTURE_HPP_ */
