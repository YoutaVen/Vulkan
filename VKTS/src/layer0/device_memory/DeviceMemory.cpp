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

#include "DeviceMemory.hpp"

namespace vkts
{

DeviceMemory::DeviceMemory(const VkDevice device, const VkMemoryAllocateInfo& memoryAllocInfo, const uint32_t memoryTypeCount, const VkMemoryType* memoryTypes, const VkMemoryPropertyFlags memoryPropertyFlags, const VkDeviceMemory deviceMemory) :
    IDeviceMemory(), device(device), memoryAllocInfo(memoryAllocInfo), allMemoryTypes(0), memoryPropertyFlags(memoryPropertyFlags), deviceMemory(deviceMemory), data(nullptr), mapped(VK_FALSE)
{
    if (memoryTypes)
    {
        for (uint32_t i = 0; i < memoryTypeCount; i++)
        {
            allMemoryTypes.push_back(memoryTypes[i]);
        }
    }
}

DeviceMemory::~DeviceMemory()
{
    destroy();
}

//
// IDeviceMemory
//

const VkDevice DeviceMemory::getDevice() const
{
    return device;
}

const VkMemoryAllocateInfo& DeviceMemory::getMemoryAllocInfo() const
{
    return memoryAllocInfo;
}

VkDeviceSize DeviceMemory::getAllocationSize() const
{
    return memoryAllocInfo.allocationSize;
}

uint32_t DeviceMemory::getMemoryTypeIndex() const
{
    return memoryAllocInfo.memoryTypeIndex;
}

VkMemoryType DeviceMemory::getMemoryType() const
{
    return allMemoryTypes[memoryAllocInfo.memoryTypeIndex];
}

uint32_t DeviceMemory::getMemoryTypeCount() const
{
    return (uint32_t) allMemoryTypes.size();
}

const VkMemoryType* DeviceMemory::getMemoryTypes() const
{
    if (allMemoryTypes.size() == 0)
    {
        return nullptr;
    }

    return &allMemoryTypes[0];
}

VkMemoryPropertyFlags DeviceMemory::getMemoryPropertyFlags() const
{
    return memoryPropertyFlags;
}

const VkDeviceMemory DeviceMemory::getDeviceMemory() const
{
    return deviceMemory;
}

VkResult DeviceMemory::mapMemory(const VkDeviceSize offset, const VkDeviceSize size, const VkMemoryMapFlags flags)
{
    if (!(memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
    {
        return VK_ERROR_MEMORY_MAP_FAILED;
    }

    if (mapped)
    {
        unmapMemory();
    }

    auto result = vkMapMemory(device, deviceMemory, offset, size, flags, &data);

    if (result == VK_SUCCESS)
    {
        mapped = VK_TRUE;
    }
    else
    {
        data = nullptr;
    }

    return result;
}

void* DeviceMemory::getMemory()
{
    return data;
}

void DeviceMemory::unmapMemory()
{
    if (!(memoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
    {
        return;
    }

    if (mapped)
    {
        vkUnmapMemory(device, deviceMemory);

        data = nullptr;

        mapped = VK_FALSE;
    }
}

VkResult DeviceMemory::upload(const VkDeviceSize offset, const VkDeviceSize size, const VkMemoryMapFlags flags, const void* uploadData, const size_t uploadDataSize)
{
    auto result = mapMemory(offset, size, flags);

    if (result != VK_SUCCESS)
    {
        return result;
    }

    memcpy(data, uploadData, uploadDataSize);

    unmapMemory();

    return result;
}

//
// IDestroyable
//

void DeviceMemory::destroy()
{
    if (deviceMemory)
    {
        if (mapped)
        {
            unmapMemory();
        }

        vkFreeMemory(device, deviceMemory, nullptr);

        deviceMemory = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
