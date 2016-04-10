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

#include "Queue.hpp"

namespace vkts
{

IQueueSP VKTS_APIENTRY queueGet(const VkDevice device, const uint32_t queueFamilyIndex, const uint32_t queueIndex)
{
    if (!device)
    {
        return IQueueSP();
    }

    VkQueue queue;

    vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, &queue);

    return IQueueSP(new Queue(device, queueFamilyIndex, queueIndex, queue));
}

VkBool32 VKTS_APIENTRY queueGetFamilyIndex(const std::vector<VkQueueFamilyProperties>& allQueueFamilyProperties, const VkQueueFlags queueFlags, const uint32_t queueIndex, const std::vector<VkBool32>* filter, uint32_t& queueFamilyIndex)
{
    for (uint32_t queueFamilyIndexWalker = 0; queueFamilyIndexWalker < allQueueFamilyProperties.size(); queueFamilyIndexWalker++)
    {
        if ((allQueueFamilyProperties[queueFamilyIndexWalker].queueFlags & queueFlags) && allQueueFamilyProperties[queueFamilyIndexWalker].queueCount > queueIndex)
        {
            if (!filter || (filter && (*filter)[queueFamilyIndexWalker]))
            {
                queueFamilyIndex = queueFamilyIndexWalker;

                return VK_TRUE;
            }
        }
    }

    logPrint(VKTS_LOG_ERROR, "Example: Could not find matching physical device queue properties.");

    return VK_FALSE;
}

}
