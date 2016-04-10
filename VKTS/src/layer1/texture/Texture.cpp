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

#include "Texture.hpp"

namespace vkts
{

Texture::Texture(const IInitialResourcesSP& initialResources, const std::string& name, const IMemoryImageSP& memoryImage, const ISamplerSP& sampler, const IImageViewSP& imageView) :
    ITexture(), initialResources(initialResources), name(name), memoryImage(memoryImage), sampler(sampler), imageView(imageView)
{
}

Texture::~Texture()
{
    destroy();
}

//
// ITexture
//

const IInitialResourcesSP& Texture::getInitialResources() const
{
    return initialResources;
}

const std::string& Texture::getName() const
{
    return name;
}

const IMemoryImageSP& Texture::getMemoryImage() const
{
    return memoryImage;
}

const ISamplerSP& Texture::getSampler() const
{
    return sampler;
}

const IImageViewSP& Texture::getImageView() const
{
    return imageView;
}

//
// IDestroyable
//

void Texture::destroy()
{
    if (imageView.get())
    {
        imageView->destroy();
    }

    if (sampler.get())
    {
        sampler->destroy();
    }

    if (memoryImage.get())
    {
    	memoryImage->destroy();
    }
}

} /* namespace vkts */
