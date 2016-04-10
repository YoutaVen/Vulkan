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

#include "CommandPool.hpp"

namespace vkts
{

CommandPool::CommandPool(const VkDevice device, const VkCommandPoolCreateFlags flags, const uint32_t queueFamilyIndex, const VkCommandPool commandPool) :
    ICommandPool(), device(device), commandPoolCreateInfo{VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, nullptr, flags, queueFamilyIndex}, commandPool(commandPool)
{
}

CommandPool::~CommandPool()
{
    destroy();
}

//
// ICommandPool
//

const VkDevice CommandPool::getDevice() const
{
    return device;
}

const VkCommandPoolCreateInfo& CommandPool::getCommandPoolCreateInfo() const
{
    return commandPoolCreateInfo;
}

uint32_t CommandPool::getQueueFamilyIndex() const
{
    return commandPoolCreateInfo.queueFamilyIndex;
}

VkCommandPoolCreateFlags CommandPool::getFlags() const
{
    return commandPoolCreateInfo.flags;
}

const VkCommandPool CommandPool::getCmdPool() const
{
    return commandPool;
}

//
// IDestroyable
//

void CommandPool::destroy()
{
    if (commandPool)
    {
        vkDestroyCommandPool(device, commandPool, nullptr);

        commandPool = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
