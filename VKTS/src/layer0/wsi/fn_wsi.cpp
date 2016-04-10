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

#include "Surface.hpp"
#include "Swapchain.hpp"

#include "fn_wsi_internal.hpp"

namespace vkts
{

VkBool32 VKTS_APIENTRY wsiGatherNeededInstanceExtensions()
{
    VkResult result;

    //

    uint32_t propertyCount = 0;

    result = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);

    if (result != VK_SUCCESS || propertyCount == 0)
    {
        return VK_FALSE;
    }

    std::vector<VkExtensionProperties> allInstanceExtensionProperties(propertyCount);

    result = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, &allInstanceExtensionProperties[0]);

    if (result != VK_SUCCESS)
    {
        return VK_FALSE;
    }

    //

    VkBool32 extensionFound = VK_FALSE;

    for (uint32_t i = 0; i < propertyCount; i++)
    {
        if (strcmp(VK_KHR_SURFACE_EXTENSION_NAME, allInstanceExtensionProperties[i].extensionName) == 0)
        {
        	if (!_extensionAddInstanceExtensions(VK_KHR_SURFACE_EXTENSION_NAME))
        	{
        		return VK_FALSE;
        	}

            extensionFound = VK_TRUE;

            break;
        }
    }

    if (!extensionFound)
    {
        return VK_FALSE;
    }

    //

    return _wsiGatherNeededInstanceExtensions(allInstanceExtensionProperties);
}

VkBool32 VKTS_APIENTRY wsiInitInstanceExtensions(const VkInstance instance)
{
    if (!instance)
    {
        return VK_FALSE;
    }

    return _wsiInitInstanceExtensions(instance);
}

//

VkBool32 VKTS_APIENTRY wsiGatherNeededDeviceExtensions(const VkPhysicalDevice physicalDevice)
{
    if (!physicalDevice)
    {
        return VK_FALSE;
    }

    VkResult result;

    //

    uint32_t propertyCount = 0;

    result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &propertyCount, nullptr);

    if (result != VK_SUCCESS || propertyCount == 0)
    {
        return VK_FALSE;
    }

    std::vector<VkExtensionProperties> allDeviceExtensionProperties( propertyCount);

    result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &propertyCount, &allDeviceExtensionProperties[0]);

    if (result != VK_SUCCESS)
    {
        return VK_FALSE;
    }

    //

    VkBool32 extensionFound = VK_FALSE;

    for (uint32_t i = 0; i < propertyCount; i++)
    {
        if (strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, allDeviceExtensionProperties[i].extensionName) == 0)
        {
        	if (!_extensionAddDeviceExtensions(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
        	{
        		return VK_FALSE;
        	}

            extensionFound = VK_TRUE;

            break;
        }
    }

    return extensionFound;
}

VkBool32 VKTS_APIENTRY wsiInitDeviceExtensions(const VkDevice device)
{
    if (!device)
    {
        return VK_FALSE;
    }

    return VK_TRUE;
}

//

VkResult VKTS_APIENTRY wsiGetPhysicalDeviceSurfaceSupport(const VkPhysicalDevice physicalDevice, const VkSurfaceKHR surface, const uint32_t queueFamilyIndexCount, std::vector<VkBool32>& supported)
{
    VkResult result;

    //

    supported.clear();

    VkBool32 isSupported;

    for (uint32_t currentQueueFamilyIndex = 0; currentQueueFamilyIndex < queueFamilyIndexCount; currentQueueFamilyIndex++)
    {
        result = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, currentQueueFamilyIndex, surface, &isSupported);

        if (result != VK_SUCCESS)
        {
            supported.clear();

            return result;
        }

        supported.push_back(isSupported);
    }

    return result;
}

/**
 *
 * @ThreadSafe
 */
ISurfaceSP VKTS_APIENTRY wsiSurfaceCreate(const VkInstance instance, VKTS_NATIVE_DISPLAY nativeDisplay, VKTS_NATIVE_WINDOW nativeWindow)
{
	VkSurfaceKHR surface = _wsiSurfaceCreate(instance, nativeDisplay, nativeWindow);

	if (surface == VK_NULL_HANDLE)
	{
		return ISurfaceSP();
	}

	auto newInstance = new Surface(instance, nativeDisplay, nativeWindow, surface);

	if (!newInstance)
	{
		vkDestroySurfaceKHR(instance, surface, nullptr);

		return ISurfaceSP();
	}


    return ISurfaceSP(newInstance);
}

/**
 *
 * @ThreadSafe
 */
ISwapchainSP VKTS_APIENTRY wsiSwapchainCreate(const VkPhysicalDevice physicalDevice, const VkDevice device, const VkSwapchainCreateFlagsKHR flags, const VkSurfaceKHR surface, const uint32_t minImageCount, const uint32_t imageArrayLayers, const VkImageUsageFlags imageUsage, const VkSharingMode imageSharingMode, const uint32_t queueFamilyIndexCount, const uint32_t* queueFamilyIndices, const VkCompositeAlphaFlagBitsKHR compositeAlpha, const VkBool32 clipped, const VkSwapchainKHR oldSwapchain)
{
    if (!physicalDevice || !device)
    {
        return ISwapchainSP();
    }

    VkResult result;

    //

    VkSurfaceCapabilitiesKHR surfaceCapabilities;

    result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

    if (result != VK_SUCCESS)
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Wsi: Could not get surface properties.");

        return ISwapchainSP();
    }

    VkSurfaceTransformFlagBitsKHR preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

    if (!(surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR))
    {
        preTransform = surfaceCapabilities.currentTransform;
    }

    uint32_t surfacePresentModesCount;

    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &surfacePresentModesCount, nullptr);

    if (result != VK_SUCCESS)
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Wsi: Could not get surface present mode count.");

        return ISwapchainSP();
    }

    std::unique_ptr<VkPresentModeKHR[]> surfacePresentModes = std::unique_ptr<VkPresentModeKHR[]> (new VkPresentModeKHR[surfacePresentModesCount]);

    if (!surfacePresentModes.get())
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Wsi: Could not create surface present modes.");

        return ISwapchainSP();
    }

    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &surfacePresentModesCount, &surfacePresentModes[0]);

    if (result != VK_SUCCESS)
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Wsi: Could not get surface present modes.");

        return ISwapchainSP();
    }

	// Regarding specification, this present mode has to be supported.
	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

    uint32_t surfaceFormatsCount;

    result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, nullptr);

    if (result != VK_SUCCESS || surfaceFormatsCount == 0)
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Wsi: Could not get surface formats count.");

        return ISwapchainSP();
    }

    std::unique_ptr<VkSurfaceFormatKHR[]> surfaceFormats = std::unique_ptr <VkSurfaceFormatKHR[]>(new VkSurfaceFormatKHR[surfaceFormatsCount]);

    if (!surfaceFormats.get())
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Wsi: Could not create surface formats.");

        return ISwapchainSP();
    }

    result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, &surfaceFormats[0]);

    if (result != VK_SUCCESS)
    {
        vkts::logPrint(VKTS_LOG_ERROR, "Wsi: Could not get surface formats.");

        return ISwapchainSP();
    }

    VkFormat imageFormat = surfaceFormats[0].format;
    VkColorSpaceKHR imageColorSpace = surfaceFormats[0].colorSpace;

    VkSwapchainCreateInfoKHR swapchainCreateInfo;

    memset(&swapchainCreateInfo, 0, sizeof(VkSwapchainCreateInfoKHR));

    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

    swapchainCreateInfo.flags = flags;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = minImageCount;
    swapchainCreateInfo.imageFormat = imageFormat;
    swapchainCreateInfo.imageColorSpace = imageColorSpace;
    swapchainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
    swapchainCreateInfo.imageArrayLayers = imageArrayLayers;
    swapchainCreateInfo.imageUsage = imageUsage;
    swapchainCreateInfo.imageSharingMode = imageSharingMode;
    swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndexCount;
    swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    swapchainCreateInfo.preTransform = preTransform;
    swapchainCreateInfo.compositeAlpha = compositeAlpha;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = clipped;
    swapchainCreateInfo.oldSwapchain = oldSwapchain;

    VkSwapchainKHR swapchain;

    result = vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Wsi: Could not create swapchain.");

        return ISwapchainSP();
    }

    auto newInstance = new Swapchain(device, flags, surface, minImageCount, imageFormat, imageColorSpace, surfaceCapabilities.currentExtent, imageArrayLayers, imageUsage, imageSharingMode, queueFamilyIndexCount, queueFamilyIndices, preTransform, compositeAlpha, presentMode, clipped, oldSwapchain, swapchain);

    if (!newInstance)
    {
        vkDestroySwapchainKHR(device, swapchain, nullptr);

        return ISwapchainSP();
    }

    return ISwapchainSP(newInstance);
}

}
