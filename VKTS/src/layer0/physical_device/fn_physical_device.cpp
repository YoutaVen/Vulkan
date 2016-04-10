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

#include "PhysicalDevice.hpp"

namespace vkts
{

IPhysicalDeviceSP VKTS_APIENTRY physicalDeviceCreate(const VkInstance instance, const uint32_t index)
{
    if (!instance)
    {
        return IPhysicalDeviceSP();
    }

    uint32_t physicalDeviceCount;

    VkResult result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount,
                                                 nullptr);

    if (result != VK_SUCCESS || index >= physicalDeviceCount)
    {
        logPrint(VKTS_LOG_ERROR, "PhysicalDevice: Could not get physical devices count.");

        return IPhysicalDeviceSP();
    }

    auto physicalDevices = std::unique_ptr<VkPhysicalDevice[]>(new VkPhysicalDevice[physicalDeviceCount]);

    if (!physicalDevices.get())
    {
        return IPhysicalDeviceSP();
    }

    result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, &physicalDevices[0]);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "PhysicalDevice: Could not enumerate physical devices.");

        return IPhysicalDeviceSP();
    }

    return IPhysicalDeviceSP(new PhysicalDevice(physicalDevices[index], index, instance));
}

}
