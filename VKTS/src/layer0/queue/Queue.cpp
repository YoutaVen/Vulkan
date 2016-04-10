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

#include "Queue.hpp"

namespace vkts
{

Queue::Queue(const VkDevice device, const uint32_t queueFamilyIndex, const uint32_t queueIndex, const VkQueue queue) :
    IQueue(), device(device), queueFamilyIndex(queueFamilyIndex), queueIndex(queueIndex), queue(queue)
{
}

Queue::~Queue()
{
}

//
// IQueue
//

VkResult Queue::submit(const uint32_t submitCount, const VkSubmitInfo* submits, const VkFence fence) const
{
    return vkQueueSubmit(queue, submitCount, submits, fence);
}

VkResult Queue::waitIdle() const
{
    return vkQueueWaitIdle(queue);
}

VkResult Queue::bindSparse(const uint32_t bindInfoCount, const VkBindSparseInfo* bindInfo, const VkFence fence) const
{
    return vkQueueBindSparse(queue, bindInfoCount, bindInfo, fence);
}

const VkDevice Queue::getDevice() const
{
    return device;
}

uint32_t Queue::getQueueFamilyIndex() const
{
    return queueFamilyIndex;
}

uint32_t Queue::getQueueIndex() const
{
    return queueIndex;
}

const VkQueue Queue::getQueue() const
{
    return queue;
}

} /* namespace vkts */
