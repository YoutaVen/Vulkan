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

#include "Device.hpp"

namespace vkts
{

Device::Device(const VkPhysicalDevice physicalDevice, const VkDeviceCreateFlags flags, const uint32_t queueCreateInfoCount, const VkDeviceQueueCreateInfo* queueCreateInfos, const uint32_t enabledLayerNameCount, const char* const* enabledLayerNames, const uint32_t enabledExtensionNameCount, const char* const* enabledExtensionNames, const VkPhysicalDeviceFeatures* enabledFeatures, const VkDevice device) :
    IDevice(), physicalDevice(physicalDevice), deviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr, flags, queueCreateInfoCount, nullptr, enabledLayerNameCount, enabledLayerNames, enabledExtensionNameCount, enabledExtensionNames, nullptr}, allDeviceQueueCreateInfos(), physicalDeviceFeatures(), device(device)
{
    for (uint32_t i = 0; i < queueCreateInfoCount; i++)
    {
        allDeviceQueueCreateInfos.push_back(queueCreateInfos[i]);
    }

    if (queueCreateInfoCount > 0)
    {
        deviceCreateInfo.pQueueCreateInfos = &(allDeviceQueueCreateInfos[0]);
    }

    if (enabledFeatures)
    {
        memcpy(&physicalDeviceFeatures, enabledFeatures, sizeof(VkPhysicalDeviceFeatures));

        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
    }
}

Device::~Device()
{
    destroy();
}

//
// IDevice
//

const VkPhysicalDevice Device::getPhysicalDevice() const
{
    return physicalDevice;
}

const VkDeviceCreateInfo& Device::getDeviceCreateInfo() const
{
    return deviceCreateInfo;
}

VkDeviceCreateFlags Device::getFlags() const
{
    return deviceCreateInfo.flags;
}

uint32_t Device::getQueueCreateInfoCount() const
{
    return deviceCreateInfo.queueCreateInfoCount;
}

const VkDeviceQueueCreateInfo* Device::getQueueCreateInfos() const
{
    return deviceCreateInfo.pQueueCreateInfos;
}

uint32_t Device::getEnabledLayerCount() const
{
    return deviceCreateInfo.enabledLayerCount;
}

const char* const* Device::getEnabledLayerNames() const
{
    return deviceCreateInfo.ppEnabledLayerNames;
}

uint32_t Device::getEnabledExtensionCount() const
{
    return deviceCreateInfo.enabledExtensionCount;
}

const char* const* Device::getEnabledExtensionNames() const
{
    return deviceCreateInfo.ppEnabledExtensionNames;
}

const VkPhysicalDeviceFeatures* Device::getEnabledFeatures() const
{
    return deviceCreateInfo.pEnabledFeatures;
}

const VkDevice Device::getDevice() const
{
    return device;
}

//
// IDestroyable
//

void Device::destroy()
{
    if (device)
    {
        vkDestroyDevice(device, nullptr);

        device = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
