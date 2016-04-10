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

#include "../../fn_internal.hpp"

// VK_EXT_DEBUG_REPORT_EXTENSION_NAME

PFN_vkCreateDebugReportCallbackEXT vktsCreateDebugReportCallbackEXT;
PFN_vkDestroyDebugReportCallbackEXT vktsDestroyDebugReportCallbackEXT;
PFN_vkDebugReportMessageEXT vktsDebugReportMessageEXT;

namespace vkts
{

VkBool32 VKTS_APIENTRY debugGatherNeededInstanceExtensions()
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
        if (strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, allInstanceExtensionProperties[i].extensionName) == 0)
        {
        	if (!_extensionAddInstanceExtensions(VK_EXT_DEBUG_REPORT_EXTENSION_NAME))
        	{
        		return VK_FALSE;
        	}

            extensionFound = VK_TRUE;

            break;
        }
    }

    return extensionFound;
}

VkBool32 VKTS_APIENTRY debugInitInstanceExtensions(const VkInstance instance)
{
    if (!instance)
    {
        return VK_FALSE;
    }

    GET_INSTANCE_PROC_ADDR(instance, CreateDebugReportCallbackEXT);
    GET_INSTANCE_PROC_ADDR(instance, DestroyDebugReportCallbackEXT);
    GET_INSTANCE_PROC_ADDR(instance, DebugReportMessageEXT);

    return VK_TRUE;
}

//

static std::mutex g_debugMutex;

VKAPI_ATTR VkBool32 VKAPI_CALL vktsDebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	std::lock_guard<std::mutex> lockGuard(g_debugMutex);

	logPrint(VKTS_LOG_INFO, "Debug: Flags 0x%x ObjectType %d Object %u Location %u MessageCode %d LayerPrefix '%s' Message '%s' UserData #%p", flags, objectType, object, location, messageCode, pLayerPrefix, pMessage, pUserData);

    return VK_TRUE;
}

//

static VkDebugReportCallbackEXT g_debugReportCallback = VK_NULL_HANDLE;

VkBool32 VKTS_APIENTRY debugCreateDebugReportCallback(const VkInstance instance, const VkDebugReportFlagsEXT flags)
{
	if (instance == VK_NULL_HANDLE || g_debugReportCallback != VK_NULL_HANDLE)
	{
		return VK_FALSE;
	}

	VkDebugReportCallbackCreateInfoEXT debugReportCallbackCreateInfo;

	memset(&debugReportCallbackCreateInfo, 0, sizeof(VkDebugReportCallbackCreateInfoEXT));

	debugReportCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	debugReportCallbackCreateInfo.flags = flags;
	debugReportCallbackCreateInfo.pfnCallback = &vktsDebugReportCallback;
	debugReportCallbackCreateInfo.pUserData = nullptr;

	VkResult result = vktsCreateDebugReportCallbackEXT(instance, &debugReportCallbackCreateInfo, nullptr, &g_debugReportCallback);

	if (result != VK_SUCCESS)
	{
		return VK_FALSE;
	}

	return VK_TRUE;
}

void VKTS_APIENTRY debugDestroyDebugReportCallback(const VkInstance instance)
{
	if (instance == VK_NULL_HANDLE || g_debugReportCallback == VK_NULL_HANDLE)
	{
		return;
	}

	vktsDestroyDebugReportCallbackEXT(instance, g_debugReportCallback, nullptr);

	g_debugReportCallback = VK_NULL_HANDLE;
}

}
