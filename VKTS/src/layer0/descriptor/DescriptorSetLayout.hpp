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

#ifndef VKTS_DESCRIPTORSETLAYOUT_HPP_
#define VKTS_DESCRIPTORSETLAYOUT_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class DescriptorSetLayout: public IDescriptorSetLayout
{

private:

    const VkDevice device;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;

    VkDescriptorSetLayout descriptorSetLayout;

    std::vector<VkDescriptorSetLayoutBinding> allDescriptorSetLayoutBindings;

public:

    DescriptorSetLayout() = delete;
    DescriptorSetLayout(const VkDevice device, const VkDescriptorSetLayoutCreateFlags flags, const uint32_t bindingCount, const VkDescriptorSetLayoutBinding* binding, const VkDescriptorSetLayout descriptorSetLayout);
    DescriptorSetLayout(const DescriptorSetLayout& other) = delete;
    DescriptorSetLayout(DescriptorSetLayout&& other) = delete;
    virtual ~DescriptorSetLayout();

    DescriptorSetLayout& operator =(const DescriptorSetLayout& other) = delete;

    DescriptorSetLayout& operator =(DescriptorSetLayout && other) = delete;

    //
    // IDescriptorSetLayout
    //

    virtual const VkDevice getDevice() const override;

    virtual const VkDescriptorSetLayoutCreateInfo& getDescriptorSetLayoutCreateInfo() const override;

    virtual VkDescriptorSetLayoutCreateFlags getFlags() const override;

    virtual uint32_t getBindingCount() const override;

    virtual const VkDescriptorSetLayoutBinding* getBindings() const override;

    virtual const VkDescriptorSetLayout getDescriptorSetLayout() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_DESCRIPTORSETLAYOUT_HPP_ */
