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

#ifndef VKTS_DESCRIPTORPOOL_HPP_
#define VKTS_DESCRIPTORPOOL_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class DescriptorPool: public IDescriptorPool
{

private:

    const VkDevice device;

    std::vector<VkDescriptorPoolSize> allPoolSizes;
    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;

    VkDescriptorPool descriptorPool;

public:

    DescriptorPool() = delete;
    DescriptorPool(const VkDevice device, const VkDescriptorPoolCreateFlags flags, const uint32_t maxSets, const uint32_t poolSizeCount, const VkDescriptorPoolSize* poolSizes, const VkDescriptorPool descriptorPool);
    DescriptorPool(const DescriptorPool& other) = delete;
    DescriptorPool(DescriptorPool&& other) = delete;
    virtual ~DescriptorPool();

    DescriptorPool& operator =(const DescriptorPool& other) = delete;
    DescriptorPool& operator =(DescriptorPool && other) = delete;

    //
    // IDescriptorPool
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkDescriptorPoolCreateInfo* getDescriptorPoolCreateInfo() const override;

    virtual VkDescriptorPoolCreateFlags getFlags() const override;

    virtual uint32_t getMaxSets() const override;

    virtual uint32_t getPoolSizeCount() const override;

    virtual const VkDescriptorPoolSize* getPoolSizes() const override;

    virtual const VkDescriptorPool getDescriptorPool() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_DESCRIPTORPOOL_HPP_ */
