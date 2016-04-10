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

#include "Texture.hpp"

namespace vkts
{

ITextureSP VKTS_APIENTRY textureCreate(const IInitialResourcesSP& initialResources, const std::string& name, const VkBool32 mipmap, const IMemoryImageSP& memoryImage, const VkSamplerCreateInfo& samplerCreateInfo, const VkImageViewCreateInfo& imageViewCreateInfo)
{
    if (!memoryImage.get())
    {
        return ITextureSP();
    }

    //

    VkSamplerCreateInfo finalSamplerCreateInfo;

    memcpy(&finalSamplerCreateInfo, &samplerCreateInfo, sizeof(VkImageViewCreateInfo));

    finalSamplerCreateInfo.maxLod = mipmap ? (float) memoryImage->getImageData()->getMipLevels() : 0.0f;

    auto sampler = samplerCreate(initialResources->getDevice()->getDevice(), finalSamplerCreateInfo.flags, finalSamplerCreateInfo.magFilter, finalSamplerCreateInfo.minFilter, finalSamplerCreateInfo.mipmapMode, samplerCreateInfo.addressModeU, samplerCreateInfo.addressModeV, finalSamplerCreateInfo.addressModeW, finalSamplerCreateInfo.mipLodBias, finalSamplerCreateInfo.anisotropyEnable, finalSamplerCreateInfo.maxAnisotropy, finalSamplerCreateInfo.compareEnable, finalSamplerCreateInfo.compareOp, finalSamplerCreateInfo.minLod, finalSamplerCreateInfo.maxLod, finalSamplerCreateInfo.borderColor, finalSamplerCreateInfo.unnormalizedCoordinates);

    if (!sampler.get())
    {
        logPrint(VKTS_LOG_ERROR, "Texture: Could not create sampler.");

        return VK_FALSE;
    }

    //

    VkImageViewCreateInfo finalImageViewCreateInfo;

    memcpy(&finalImageViewCreateInfo, &imageViewCreateInfo, sizeof(VkImageViewCreateInfo));

    finalImageViewCreateInfo.image = memoryImage->getImage()->getImage();
    finalImageViewCreateInfo.format = memoryImage->getImage()->getFormat();
    finalImageViewCreateInfo.subresourceRange.levelCount =  mipmap ? memoryImage->getImageData()->getMipLevels() : 1;

    auto imageView = imageViewCreate(initialResources->getDevice()->getDevice(), finalImageViewCreateInfo.flags, finalImageViewCreateInfo.image, finalImageViewCreateInfo.viewType, finalImageViewCreateInfo.format, finalImageViewCreateInfo.components, finalImageViewCreateInfo.subresourceRange);

    if (!imageView.get())
    {
        logPrint(VKTS_LOG_ERROR, "Texture: Could not create image view.");

        return VK_FALSE;
    }

    //

    auto newInstance = new Texture(initialResources, name, memoryImage, sampler, imageView);

    if (!newInstance)
    {
        newInstance->destroy();

        return ITextureSP();
    }

    return ITextureSP(newInstance);
}

}
