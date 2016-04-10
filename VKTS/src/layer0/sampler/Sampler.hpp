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

#ifndef VKTS_SAMPLER_HPP_
#define VKTS_SAMPLER_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Sampler: public ISampler
{

private:

    const VkDevice device;

    const VkSamplerCreateInfo samplerCreateInfo;

    VkSampler sampler;

public:

    Sampler() = delete;
    Sampler(const VkDevice device, const VkSamplerCreateFlags flags, const VkFilter magFilter, const VkFilter minFilter, const VkSamplerMipmapMode mipmapMode, const VkSamplerAddressMode addressModeU, const VkSamplerAddressMode addressModeV, const VkSamplerAddressMode addressModeW, const float mipLodBias, const VkBool32 anisotropyEnable, const float maxAnisotropy, const VkBool32 compareEnable, const VkCompareOp compareOp, const float minLod, const float maxLod, const VkBorderColor borderColor, const VkBool32 unnormalizedCoordinates, const VkSampler sampler);
    Sampler(const Sampler& other) = delete;
    Sampler(Sampler&& other) = delete;
    virtual ~Sampler();

    Sampler& operator =(const Sampler& other) = delete;

    Sampler& operator =(Sampler && other) = delete;

    //
    // ISampler
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkSamplerCreateInfo& getSamplerCreateInfo() const override;

    virtual VkSamplerCreateFlags getFlags() const override;

    virtual VkFilter getMagFilter() const override;

    virtual VkFilter getMinFilter() const override;

    virtual VkSamplerMipmapMode getMipmapMode() const override;

    virtual VkSamplerAddressMode getAddressModeU() const override;

    virtual VkSamplerAddressMode getAddressModeV() const override;

    virtual VkSamplerAddressMode getAddressModeW() const override;

    virtual float getMipLodBias() const override;

    virtual VkBool32 getAnisotropyEnable() const override;

    virtual float getMaxAnisotropy() const override;

    virtual VkBool32 getCompareEnable() const override;

    virtual VkCompareOp getCompareOp() const override;

    virtual float getMinLod() const override;

    virtual float getMaxLod() const override;

    virtual VkBorderColor getBorderColor() const override;

    virtual VkBool32 getUnnormalizedCoordinates() const override;

    virtual const VkSampler getSampler() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_SAMPLER_HPP_ */
