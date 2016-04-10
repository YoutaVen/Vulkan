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

#include "Sampler.hpp"

namespace vkts
{

Sampler::Sampler(const VkDevice device, const VkSamplerCreateFlags flags, const VkFilter magFilter, const VkFilter minFilter, const VkSamplerMipmapMode mipmapMode, const VkSamplerAddressMode addressModeU, const VkSamplerAddressMode addressModeV, const VkSamplerAddressMode addressModeW, const float mipLodBias, const VkBool32 anisotropyEnable, const float maxAnisotropy, const VkBool32 compareEnable, const VkCompareOp compareOp, const float minLod, const float maxLod, const VkBorderColor borderColor, const VkBool32 unnormalizedCoordinates, const VkSampler sampler) :
    ISampler(), device(device), samplerCreateInfo{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr, flags, magFilter, minFilter, mipmapMode, addressModeU, addressModeV, addressModeW, mipLodBias, anisotropyEnable, maxAnisotropy, compareEnable, compareOp, minLod, maxLod, borderColor, unnormalizedCoordinates}, sampler(sampler)
{
}

Sampler::~Sampler()
{
    destroy();
}

//
// ISampler
//

const VkDevice Sampler::getDevice() const
{
    return device;
}

const VkSamplerCreateInfo& Sampler::getSamplerCreateInfo() const
{
    return samplerCreateInfo;
}

VkSamplerCreateFlags Sampler::getFlags() const
{
    return samplerCreateInfo.flags;
}

VkFilter Sampler::getMagFilter() const
{
    return samplerCreateInfo.magFilter;
}

VkFilter Sampler::getMinFilter() const
{
    return samplerCreateInfo.minFilter;
}

VkSamplerMipmapMode Sampler::getMipmapMode() const
{
    return samplerCreateInfo.mipmapMode;
}

VkSamplerAddressMode Sampler::getAddressModeU() const
{
    return samplerCreateInfo.addressModeU;
}

VkSamplerAddressMode Sampler::getAddressModeV() const
{
    return samplerCreateInfo.addressModeV;
}

VkSamplerAddressMode Sampler::getAddressModeW() const
{
    return samplerCreateInfo.addressModeW;
}

float Sampler::getMipLodBias() const
{
    return samplerCreateInfo.mipLodBias;
}

VkBool32 Sampler::getAnisotropyEnable() const
{
    return samplerCreateInfo.anisotropyEnable;
}

float Sampler::getMaxAnisotropy() const
{
    return samplerCreateInfo.maxAnisotropy;
}

VkBool32 Sampler::getCompareEnable() const
{
    return samplerCreateInfo.compareEnable;
}

VkCompareOp Sampler::getCompareOp() const
{
    return samplerCreateInfo.compareOp;
}

float Sampler::getMinLod() const
{
    return samplerCreateInfo.minLod;
}

float Sampler::getMaxLod() const
{
    return samplerCreateInfo.maxLod;
}

VkBorderColor Sampler::getBorderColor() const
{
    return samplerCreateInfo.borderColor;
}

VkBool32 Sampler::getUnnormalizedCoordinates() const
{
    return samplerCreateInfo.unnormalizedCoordinates;
}

const VkSampler Sampler::getSampler() const
{
    return sampler;
}

//
// IDestroyable
//

void Sampler::destroy()
{
    if (sampler)
    {
        vkDestroySampler(device, sampler, nullptr);

        sampler = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
