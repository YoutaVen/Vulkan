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

#include "DescriptorSetLayout.hpp"

namespace vkts
{

DescriptorSetLayout::DescriptorSetLayout(const VkDevice device, const VkDescriptorSetLayoutCreateFlags flags, const uint32_t bindingCount, const VkDescriptorSetLayoutBinding* binding, const VkDescriptorSetLayout descriptorSetLayout) :
    IDescriptorSetLayout(), device(device), descriptorSetLayoutCreateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, nullptr, flags, bindingCount, nullptr}, descriptorSetLayout(descriptorSetLayout)
{
    for (uint32_t i = 0; i < bindingCount; i++)
    {
        allDescriptorSetLayoutBindings.push_back(binding[i]);
    }

    if (bindingCount > 0)
    {
        descriptorSetLayoutCreateInfo.pBindings = &(allDescriptorSetLayoutBindings[0]);
    }
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    destroy();
}

//
// IDescriptorSetLayout
//

const VkDevice DescriptorSetLayout::getDevice() const
{
    return device;
}

const VkDescriptorSetLayoutCreateInfo& DescriptorSetLayout::getDescriptorSetLayoutCreateInfo() const
{
    return descriptorSetLayoutCreateInfo;
}

VkDescriptorSetLayoutCreateFlags DescriptorSetLayout::getFlags() const
{
    return descriptorSetLayoutCreateInfo.flags;
}

uint32_t DescriptorSetLayout::getBindingCount() const
{
    return descriptorSetLayoutCreateInfo.bindingCount;
}

const VkDescriptorSetLayoutBinding* DescriptorSetLayout::getBindings() const
{
    return descriptorSetLayoutCreateInfo.pBindings;
}

const VkDescriptorSetLayout DescriptorSetLayout::getDescriptorSetLayout() const
{
    return descriptorSetLayout;
}

//
// IDestroyable
//

void DescriptorSetLayout::destroy()
{
    if (descriptorSetLayout)
    {
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

        descriptorSetLayout = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
