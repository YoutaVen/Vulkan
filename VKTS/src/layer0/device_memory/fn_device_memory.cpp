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

#include "DeviceMemory.hpp"

namespace vkts
{

IDeviceMemorySP VKTS_APIENTRY deviceMemoryCreate(const VkDevice device, const VkMemoryRequirements& memoryRequirements, const uint32_t memoryTypeCount, const VkMemoryType* memoryTypes, const VkMemoryPropertyFlags propertyFlags)
{
    if (!device || !memoryTypes)
    {
        return IDeviceMemorySP();
    }

    VkResult result;

    //

    VkMemoryAllocateInfo memoryAllocInfo;

    memset(&memoryAllocInfo, 0, sizeof(VkMemoryAllocateInfo));

    memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocInfo.allocationSize = memoryRequirements.size;
    memoryAllocInfo.memoryTypeIndex = 0; // Will be gathered in next function.

    if (!commonGetMemoryTypeIndex(memoryTypeCount, memoryTypes, memoryRequirements.memoryTypeBits, propertyFlags, memoryAllocInfo.memoryTypeIndex))
    {
        logPrint(VKTS_LOG_ERROR, "DeviceMemory: Could not get memory type index.");

        return IDeviceMemorySP();
    }

    VkDeviceMemory deviceMemory;

    result = vkAllocateMemory(device, &memoryAllocInfo, nullptr, &deviceMemory);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "DeviceMemory: Could not allocate device memory.");

        return IDeviceMemorySP();
    }

    auto newInstance = new DeviceMemory(device, memoryAllocInfo, memoryTypeCount, memoryTypes, propertyFlags, deviceMemory);

    if (!newInstance)
    {
        vkFreeMemory(device, deviceMemory, nullptr);

        return IDeviceMemorySP();
    }

    return IDeviceMemorySP(newInstance);
}

}
