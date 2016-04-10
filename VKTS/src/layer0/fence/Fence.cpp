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

#include "Fence.hpp"

namespace vkts
{

Fence::Fence(const VkDevice device, const VkFenceCreateFlags flags, const VkFence fence) :
    IFence(), device(device), fenceCreateInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, nullptr, flags}, fence(fence)
{
}

Fence::~Fence()
{
    destroy();
}

//
// IFence
//

VkResult Fence::resetFence() const
{
    return vkResetFences(device, 1, &fence);
}

VkResult Fence::getStatus() const
{
	return vkGetFenceStatus(device, fence);
}

VkResult Fence::waitForFence(const uint64_t timeout) const
{
    return vkWaitForFences(device, 1, &fence, VK_TRUE, timeout);
}

const VkDevice Fence::getDevice() const
{
    return device;
}

const VkFenceCreateInfo& Fence::getFenceCreateInfo() const
{
    return fenceCreateInfo;
}

VkFenceCreateFlags Fence::getFlags() const
{
    return fenceCreateInfo.flags;
}

const VkFence Fence::getFence() const
{
    return fence;
}

//
// IResetable
//

VkResult Fence::reset()
{
    return resetFence();
}

//
// IDestroyable
//

void Fence::destroy()
{
    if (fence)
    {
        vkDestroyFence(device, fence, nullptr);

        fence = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
