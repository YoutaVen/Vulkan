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

#include "DescriptorSets.hpp"

namespace vkts
{

DescriptorSets::DescriptorSets(const VkDevice device, const VkDescriptorPool descriptorPool, const uint32_t setLayoutCount, const VkDescriptorSetLayout* setLayout, const VkDescriptorSet* descriptorSet) :
    IDescriptorSets(), device(device), descriptorSetAllocateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, nullptr, descriptorPool, setLayoutCount, nullptr}, allSetLayouts(), allDescriptorSets()
{
    for (uint32_t i = 0; i < setLayoutCount; i++)
    {
        allSetLayouts.push_back(setLayout[i]);

        allDescriptorSets.push_back(descriptorSet[i]);
    }

    if (setLayoutCount > 0)
    {
        descriptorSetAllocateInfo.pSetLayouts = &allSetLayouts[0];
    }
}

DescriptorSets::~DescriptorSets()
{
    destroy();
}

//
// IDescriptorSets
//

const VkDevice DescriptorSets::getDevice() const
{
    return device;
}

const VkDescriptorPool DescriptorSets::getDescriptorPool() const
{
    return descriptorSetAllocateInfo.descriptorPool;
}

uint32_t DescriptorSets::getDescriptorSetCount() const
{
    return descriptorSetAllocateInfo.descriptorSetCount;
}

const VkDescriptorSetLayout* DescriptorSets::getSetLayouts() const
{
    return descriptorSetAllocateInfo.pSetLayouts;
}

const VkDescriptorSet* DescriptorSets::getDescriptorSets() const
{
    if (allDescriptorSets.size() > 0)
    {
        return &allDescriptorSets[0];
    }

    return nullptr;
}

void DescriptorSets::updateDescriptorSets(const uint32_t writeCount,
                                          const VkWriteDescriptorSet* descriptorWrites, const uint32_t copyCount,
                                          const VkCopyDescriptorSet* descriptorCopies) const
{
    vkUpdateDescriptorSets(device, writeCount, descriptorWrites, copyCount, descriptorCopies);
}

//
// IDestroyable
//

void DescriptorSets::destroy()
{
    allSetLayouts.clear();

    if (allDescriptorSets.size() > 0)
    {
        vkFreeDescriptorSets(device, descriptorSetAllocateInfo.descriptorPool, (uint32_t) allDescriptorSets.size(), &allDescriptorSets[0]);
    }

    allDescriptorSets.clear();
}

} /* namespace vkts */
