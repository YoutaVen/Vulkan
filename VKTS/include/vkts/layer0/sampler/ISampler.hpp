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

#ifndef VKTS_ISAMPLER_HPP_
#define VKTS_ISAMPLER_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class ISampler: public IDestroyable
{

public:

    ISampler() :
        IDestroyable()
    {
    }

    virtual ~ISampler()
    {
    }

    virtual const VkDevice getDevice() const = 0;

    virtual const VkSamplerCreateInfo& getSamplerCreateInfo() const = 0;

    virtual VkSamplerCreateFlags getFlags() const = 0;

    virtual VkFilter getMagFilter() const = 0;

    virtual VkFilter getMinFilter() const = 0;

    virtual VkSamplerMipmapMode getMipmapMode() const = 0;

    virtual VkSamplerAddressMode getAddressModeU() const = 0;

    virtual VkSamplerAddressMode getAddressModeV() const = 0;

    virtual VkSamplerAddressMode getAddressModeW() const = 0;

    virtual float getMipLodBias() const = 0;

    virtual VkBool32 getAnisotropyEnable() const = 0;

    virtual float getMaxAnisotropy() const = 0;

    virtual VkBool32 getCompareEnable() const = 0;

    virtual VkCompareOp getCompareOp() const = 0;

    virtual float getMinLod() const = 0;

    virtual float getMaxLod() const = 0;

    virtual VkBorderColor getBorderColor() const = 0;

    virtual VkBool32 getUnnormalizedCoordinates() const = 0;

    virtual const VkSampler getSampler() const = 0;

    //
    // IDestroyable
    //

    virtual void destroy() = 0;

};

typedef std::shared_ptr<ISampler> ISamplerSP;

} /* namespace vkts */

#endif /* VKTS_ISAMPLER_HPP_ */
