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

#include "PhysicalDevice.hpp"

namespace vkts
{

PhysicalDevice::PhysicalDevice(const VkPhysicalDevice physicalDevice, const uint32_t index, const VkInstance instance) :
    IPhysicalDevice(), physicalDevice(physicalDevice), index(index), instance(instance), allQueueFamilyProperties()
{
}

PhysicalDevice::~PhysicalDevice()
{
}

//
// IPhysicalDevice
//

const VkPhysicalDevice PhysicalDevice::getPhysicalDevice() const
{
    return physicalDevice;
}

uint32_t PhysicalDevice::getIndex() const
{
    return index;
}

const VkInstance PhysicalDevice::getInstance() const
{
    return instance;
}

void PhysicalDevice::getPhysicalDeviceFeatures(VkPhysicalDeviceFeatures& physicalDeviceFeature) const
{
	vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeature);
}

void PhysicalDevice::getPhysicalDeviceProperties(VkPhysicalDeviceProperties& physicalDeviceProperties) const
{
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
}

void PhysicalDevice::getPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties& physicalDeviceMemoryProperties) const
{
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemoryProperties);
}

void PhysicalDevice::getGetPhysicalDeviceFormatProperties(VkFormatProperties& formatProperties, const VkFormat format) const
{
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);
}

void PhysicalDevice::getGetPhysicalDeviceImageFormatProperties(VkImageFormatProperties& imageFormatProperties, const VkFormat format, const VkImageType type, const VkImageTiling tiling, const VkImageUsageFlags usage, const VkImageCreateFlags flags) const
{
    vkGetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, &imageFormatProperties);
}

VkBool32 PhysicalDevice::isImageTilingAvailable(const VkImageTiling imageTiling, const VkFormat format, const VkImageType type, const VkImageCreateFlags flags, const VkExtent3D& extent, const uint32_t mipLevels, const uint32_t arrayLayers, const VkSampleCountFlags sampleCounts, const VkDeviceSize resourceSize)
{
    VkFormatProperties formatProperties;

    getGetPhysicalDeviceFormatProperties(formatProperties, format);

    //

    if (imageTiling == VK_IMAGE_TILING_LINEAR && !(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT))
    {
        return VK_FALSE;
    }

    if (imageTiling == VK_IMAGE_TILING_OPTIMAL && !(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT))
    {
        return VK_FALSE;
    }

    //

    VkImageFormatProperties imageFormatProperties;

    getGetPhysicalDeviceImageFormatProperties(imageFormatProperties, format, type, imageTiling, VK_IMAGE_USAGE_SAMPLED_BIT, flags);

    if (imageFormatProperties.maxExtent.width < extent.width || imageFormatProperties.maxExtent.height < extent.height || imageFormatProperties.maxExtent.depth < extent.depth)
    {
        return VK_FALSE;
    }

    if (imageFormatProperties.maxMipLevels < mipLevels)
    {
        return VK_FALSE;
    }

    if (imageFormatProperties.maxArrayLayers < arrayLayers)
    {
        return VK_FALSE;
    }

    if (!(imageFormatProperties.sampleCounts & sampleCounts))
    {
        return VK_FALSE;
    }

    if (imageFormatProperties.maxResourceSize < resourceSize)
    {
        return VK_FALSE;
    }

    return VK_TRUE;
}

VkBool32 PhysicalDevice::getGetImageTilingAndMemoryProperty(VkImageTiling& imageTiling, VkMemoryPropertyFlags& memoryPropertyFlags, const VkFormat format, const VkImageType type, const VkImageCreateFlags flags, const VkExtent3D& extent, const uint32_t mipLevels, const uint32_t arraySize, const VkSampleCountFlags sampleCounts, const VkDeviceSize resourceSize)
{
    for (int32_t i = 0; i < 2; i++)
    {
        // Test two tiling and memory property cases.

        switch (i)
        {
            case 0:
                imageTiling = VK_IMAGE_TILING_OPTIMAL;
                memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
                break;
            case 1:
                imageTiling = VK_IMAGE_TILING_LINEAR;
                memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
                break;
        }

        if (isImageTilingAvailable(imageTiling, format, type, flags, extent, mipLevels, arraySize, sampleCounts, resourceSize))
        {
            return VK_TRUE;
        }
    }

    return VK_FALSE;
}

const std::vector<VkQueueFamilyProperties>& PhysicalDevice::getAllQueueFamilyProperties(const VkBool32 refresh)
{
    if (allQueueFamilyProperties.size() == 0 || refresh)
    {
        allQueueFamilyProperties.clear();

        uint32_t physicalDeviceQueueCount;

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueCount, nullptr);

        VkQueueFamilyProperties queueFamilyProperties;

        memset(&queueFamilyProperties, 0, sizeof(VkQueueFamilyProperties));

        for (uint32_t index = 0; index < physicalDeviceQueueCount; index++)
        {
            allQueueFamilyProperties.push_back(queueFamilyProperties);
        }

        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &physicalDeviceQueueCount, &allQueueFamilyProperties[0]);
    }

    return allQueueFamilyProperties;
}

} /* namespace vkts */
