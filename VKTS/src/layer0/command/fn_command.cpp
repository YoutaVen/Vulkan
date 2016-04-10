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

#include "CommandBuffers.hpp"
#include "CommandPool.hpp"

namespace vkts
{

ICommandPoolSP VKTS_APIENTRY commandPoolCreate(const VkDevice device, const VkCommandPoolCreateFlags flags, const uint32_t queueFamilyIndex)
{
    if (!device)
    {
        return ICommandPoolSP();
    }

    VkResult result;

    VkCommandPoolCreateInfo commandPoolCreateInfo;

    memset(&commandPoolCreateInfo, 0, sizeof(VkCommandPoolCreateInfo));

    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

    commandPoolCreateInfo.flags = flags;
    commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

    VkCommandPool commandPool;

    result = vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "CommandPool: Could not create command pool.");

        return ICommandPoolSP();
    }

    auto newInstance = new CommandPool(device, queueFamilyIndex, flags, commandPool);

    if (!newInstance)
    {
        vkDestroyCommandPool(device, commandPool, nullptr);

        return ICommandPoolSP();
    }

    return ICommandPoolSP(newInstance);
}

ICommandBuffersSP VKTS_APIENTRY commandBuffersCreate(const VkDevice device, const VkCommandPool commandPool, const VkCommandBufferLevel level, const uint32_t commandBufferCount)
{
    if (!device)
    {
        return ICommandBuffersSP();
    }

    VkResult result;

    VkCommandBufferAllocateInfo commandBufferAllocateInfo;

    memset(&commandBufferAllocateInfo, 0, sizeof(VkCommandBufferAllocateInfo));

    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

    commandBufferAllocateInfo.commandPool = commandPool;
    commandBufferAllocateInfo.level = level;
    commandBufferAllocateInfo.commandBufferCount = commandBufferCount;

    std::vector<VkCommandBuffer> allCommandBuffers;

    VkCommandBuffer dummy;

    for (uint32_t i = 0; i < commandBufferCount; i++)
    {
        allCommandBuffers.push_back(dummy);
    }

    result = vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &allCommandBuffers[0]);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "CommandBuffer: Could not create command buffer.");

        return ICommandBuffersSP();
    }

    //

    auto newInstance = new CommandBuffers(device, commandPool, level, commandBufferCount, &allCommandBuffers[0]);

    if (!newInstance)
    {
        vkFreeCommandBuffers(device, commandPool, (uint32_t)allCommandBuffers.size(), &allCommandBuffers[0]);

        return ICommandBuffersSP();
    }

    return ICommandBuffersSP(newInstance);
}

}
