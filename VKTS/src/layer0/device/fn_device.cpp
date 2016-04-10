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

#include "Device.hpp"

namespace vkts
{

VkBool32 VKTS_APIENTRY deviceExtensionsAvailable(const VkPhysicalDevice physicalDevice, const uint32_t extensionCount, const char* const* enabledExtensionNames)
{
    if (!physicalDevice)
    {
        return VK_FALSE;
    }

    if (extensionCount == 0)
    {
        return VK_TRUE;
    }

    if (!enabledExtensionNames)
    {
        return VK_FALSE;
    }

    VkResult result;

    uint32_t physicalDeviceExtensionPropertiesCount;

    result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &physicalDeviceExtensionPropertiesCount, nullptr);

    if (result != VK_SUCCESS || physicalDeviceExtensionPropertiesCount == 0)
    {
        return VK_FALSE;
    }

    std::unique_ptr<VkExtensionProperties[]> allExtensionProperties = std::unique_ptr<VkExtensionProperties[]>(new VkExtensionProperties[physicalDeviceExtensionPropertiesCount]);

    if (!allExtensionProperties.get())
    {
        return VK_FALSE;
    }

    result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &physicalDeviceExtensionPropertiesCount, &allExtensionProperties[0]);

    if (result != VK_SUCCESS)
    {
        return VK_FALSE;
    }

    for (uint32_t extensionIndex = 0; extensionIndex < extensionCount; extensionIndex++)
    {
        VkBool32 extensionFound = VK_FALSE;

        for (uint32_t i = 0; i < physicalDeviceExtensionPropertiesCount; i++)
        {
            if (strcmp(enabledExtensionNames[extensionIndex], allExtensionProperties[i].extensionName) == 0)
            {
                extensionFound = VK_TRUE;

                break;
            }
        }

        if (!extensionFound)
        {
            return VK_FALSE;
        }
    }

    return VK_TRUE;
}

IDeviceSP VKTS_APIENTRY deviceCreate(const VkPhysicalDevice physicalDevice, const VkDeviceCreateFlags flags, const uint32_t queueCreateInfoCount, const VkDeviceQueueCreateInfo* queueCreateInfos, const uint32_t enabledLayerCount, const char* const* enabledLayerNames, const uint32_t enabledExtensionCount, const char* const* enabledExtensionNames, const VkPhysicalDeviceFeatures* enabledFeatures)
{
    if (queueCreateInfoCount == 0 || !queueCreateInfos)
    {
        return IDeviceSP();
    }

    VkResult result;

    VkDeviceCreateInfo deviceCreateInfo;

    memset(&deviceCreateInfo, 0, sizeof(VkDeviceCreateInfo));

    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    deviceCreateInfo.flags = flags;
    deviceCreateInfo.queueCreateInfoCount = queueCreateInfoCount;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos;
    deviceCreateInfo.enabledLayerCount = enabledLayerCount;
    deviceCreateInfo.ppEnabledLayerNames = enabledLayerNames;
    deviceCreateInfo.enabledExtensionCount = enabledExtensionCount;
    deviceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames;
    deviceCreateInfo.pEnabledFeatures = enabledFeatures;

    VkDevice device;

    result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Device: Could not create device.");

        return IDeviceSP();
    }

    if (enabledExtensionCount > 0)
    {
        if (!deviceExtensionsAvailable(physicalDevice, enabledExtensionCount, enabledExtensionNames))
        {
            logPrint(VKTS_LOG_ERROR, "Device: Could not initialize device extension.");

            vkDestroyDevice(device, nullptr);

            return IDeviceSP();
        }
    }

    auto newInstance = new Device(physicalDevice, flags, queueCreateInfoCount, queueCreateInfos, enabledLayerCount, enabledLayerNames, enabledExtensionCount, enabledExtensionNames, enabledFeatures, device);

    if (!newInstance)
    {
        vkDestroyDevice(device, nullptr);

        return IDeviceSP();
    }

    return IDeviceSP(newInstance);
}

}
