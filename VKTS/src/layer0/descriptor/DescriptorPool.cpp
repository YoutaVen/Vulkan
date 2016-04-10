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

#include "DescriptorPool.hpp"

namespace vkts
{

DescriptorPool::DescriptorPool(const VkDevice device, const VkDescriptorPoolCreateFlags flags, const uint32_t maxSets, const uint32_t poolSizeCount, const VkDescriptorPoolSize* poolSizes, const VkDescriptorPool descriptorPool) :
    IDescriptorPool(), device(device), allPoolSizes(), descriptorPoolCreateInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, nullptr, flags, maxSets, poolSizeCount, nullptr}, descriptorPool(descriptorPool)
{
    for (uint32_t i = 0; i < poolSizeCount; i++)
    {
        allPoolSizes.push_back(poolSizes[i]);
    }

    if (poolSizeCount > 0)
    {
        descriptorPoolCreateInfo.pPoolSizes = &(allPoolSizes[0]);
    }
}

DescriptorPool::~DescriptorPool()
{
    destroy();
}

//
// IDescriptorPool
//

const VkDevice DescriptorPool::getDevice() const
{
    return device;
}

const VkDescriptorPoolCreateInfo* DescriptorPool::getDescriptorPoolCreateInfo() const
{
    return &descriptorPoolCreateInfo;
}

VkDescriptorPoolCreateFlags DescriptorPool::getFlags() const
{
    return descriptorPoolCreateInfo.flags;
}

uint32_t DescriptorPool::getMaxSets() const
{
    return descriptorPoolCreateInfo.maxSets;
}

uint32_t DescriptorPool::getPoolSizeCount() const
{
    return descriptorPoolCreateInfo.poolSizeCount;
}

const VkDescriptorPoolSize* DescriptorPool::getPoolSizes() const
{
    return descriptorPoolCreateInfo.pPoolSizes;
}

const VkDescriptorPool DescriptorPool::getDescriptorPool() const
{
    return descriptorPool;
}

//
// IDestroyable
//

void DescriptorPool::destroy()
{
    if (descriptorPool)
    {
        vkDestroyDescriptorPool(device, descriptorPool, nullptr);

        descriptorPool = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
