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

#include "DescriptorSetLayout.hpp"
#include "DescriptorPool.hpp"
#include "DescriptorSets.hpp"

namespace vkts
{

IDescriptorSetLayoutSP VKTS_APIENTRY descriptorSetLayoutCreate(const VkDevice device, const VkDescriptorSetLayoutCreateFlags flags, const uint32_t bindingCount, const VkDescriptorSetLayoutBinding* bindings)
{
    if (!device)
    {
        return IDescriptorSetLayoutSP();
    }

    VkResult result;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;

    memset(&descriptorSetLayoutCreateInfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));

    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

    descriptorSetLayoutCreateInfo.flags = flags;
    descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
    descriptorSetLayoutCreateInfo.pBindings = bindings;

    VkDescriptorSetLayout descriptorSetLayout;

    result = vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Descriptor: Could not create descriptor set layout.");

        return IDescriptorSetLayoutSP();
    }

    auto newInstance = new DescriptorSetLayout(device, flags, bindingCount, bindings, descriptorSetLayout);

    if (!newInstance)
    {
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

        return IDescriptorSetLayoutSP();
    }

    return IDescriptorSetLayoutSP(newInstance);
}

IDescriptorPoolSP VKTS_APIENTRY descriptorPoolCreate(const VkDevice device, const VkDescriptorPoolCreateFlags flags, const uint32_t maxSets, const uint32_t poolSizeCount, const VkDescriptorPoolSize* poolSizes)
{
    if (!device)
    {
        return IDescriptorPoolSP();
    }

    VkResult result;

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;

    memset(&descriptorPoolCreateInfo, 0, sizeof(VkDescriptorPoolCreateInfo));

    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

    descriptorPoolCreateInfo.flags = flags;
    descriptorPoolCreateInfo.maxSets = maxSets;
    descriptorPoolCreateInfo.poolSizeCount = poolSizeCount;
    descriptorPoolCreateInfo.pPoolSizes = poolSizes;

    VkDescriptorPool descriptorPool;

    result = vkCreateDescriptorPool(device, &descriptorPoolCreateInfo, nullptr, &descriptorPool);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Descriptor: Could not create descriptor pool.");

        return IDescriptorPoolSP();
    }

    auto newInstance = new DescriptorPool(device, flags, maxSets, poolSizeCount, poolSizes, descriptorPool);

    if (!newInstance)
    {
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);

        return IDescriptorPoolSP();
    }

    return IDescriptorPoolSP(newInstance);
}

IDescriptorSetsSP VKTS_APIENTRY descriptorSetsCreate(const VkDevice device, const VkDescriptorPool descriptorPool, const uint32_t descriptorSetCount, const VkDescriptorSetLayout* setLayouts)
{
    if (!device || !descriptorPool || descriptorSetCount == 0 || !setLayouts)
    {
        return IDescriptorSetsSP();
    }

    std::vector<VkDescriptorSet> allDescriptorSets;

    VkDescriptorSet dummy = VK_NULL_HANDLE;
    for (uint32_t i = 0; i < descriptorSetCount; i++)
    {
        allDescriptorSets.push_back(dummy);
    }

    //

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;

    memset(&descriptorSetAllocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));

    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

    descriptorSetAllocateInfo.descriptorPool = descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount;
    descriptorSetAllocateInfo.pSetLayouts = setLayouts;

    VkResult result;

    result = vkAllocateDescriptorSets(device, &descriptorSetAllocateInfo, &allDescriptorSets[0]);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Descriptor: Could not allocate descriptor sets.");

        return IDescriptorSetsSP();
    }

    auto newInstance = new DescriptorSets(device, descriptorPool, descriptorSetCount, setLayouts, &allDescriptorSets[0]);

    if (!newInstance)
    {
        vkFreeDescriptorSets(device, descriptorPool, (uint32_t) allDescriptorSets.size(), &allDescriptorSets[0]);

        return IDescriptorSetsSP();
    }

    return IDescriptorSetsSP(newInstance);
}

}
