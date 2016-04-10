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

#include "Instance.hpp"

namespace vkts
{

VkBool32 VKTS_APIENTRY instanceExtensionsAvailable(const uint32_t extensionCount, const char* const* enabledExtensionNames)
{
    if (extensionCount == 0)
    {
        return VK_TRUE;
    }

    if (!enabledExtensionNames)
    {
        return VK_FALSE;
    }

    VkResult result;

    uint32_t globalExtensionPropertiesCount;

    result = vkEnumerateInstanceExtensionProperties(nullptr, &globalExtensionPropertiesCount, nullptr);

    if (result != VK_SUCCESS || globalExtensionPropertiesCount == 0)
    {
        return VK_FALSE;
    }

    std::unique_ptr<VkExtensionProperties[]> allExtensionProperties = std::unique_ptr<VkExtensionProperties[]>(new VkExtensionProperties[globalExtensionPropertiesCount]);

    if (!allExtensionProperties.get())
    {
        return VK_FALSE;
    }

    result = vkEnumerateInstanceExtensionProperties(nullptr, &globalExtensionPropertiesCount, &allExtensionProperties[0]);

    if (result != VK_SUCCESS)
    {
        return VK_FALSE;
    }

    for (uint32_t extensionIndex = 0; extensionIndex < extensionCount; extensionIndex++)
    {
        VkBool32 extensionFound = VK_FALSE;

        for (uint32_t i = 0; i < globalExtensionPropertiesCount; i++)
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

IInstanceSP VKTS_APIENTRY instanceCreate(const char* applicationName, const uint32_t applicationVersion, const uint32_t apiVersion, const VkInstanceCreateFlags flags, const uint32_t enabledLayerCount, const char* const* enabledLayerNames, const uint32_t enabledExtensionCount, const char* const* enabledExtensionNames)
{
    if (!applicationName)
    {
        return IInstanceSP();
    }

    VkResult result;

    VkApplicationInfo applicationInfo;

    memset(&applicationInfo, 0, sizeof(VkApplicationInfo));

    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

    applicationInfo.pApplicationName = applicationName;
    applicationInfo.applicationVersion = applicationVersion;
    applicationInfo.pEngineName = VKTS_ENGINE_NAME;
    applicationInfo.engineVersion = VK_MAKE_VERSION(VKTS_ENGINE_MAJOR, VKTS_ENGINE_MINOR, VKTS_ENGINE_PATCH);
    applicationInfo.apiVersion = apiVersion;

    VkInstanceCreateInfo instanceCreateInfo;

    memset(&instanceCreateInfo, 0, sizeof(VkInstanceCreateInfo));

    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    instanceCreateInfo.flags = flags;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledLayerCount = enabledLayerCount;
    instanceCreateInfo.ppEnabledLayerNames = enabledLayerNames;
    instanceCreateInfo.enabledExtensionCount = enabledExtensionCount;
    instanceCreateInfo.ppEnabledExtensionNames = enabledExtensionNames;

    VkInstance instance;

    result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Instance: Could not create instance.");

        return IInstanceSP();
    }

    if (enabledExtensionCount > 0)
    {
        if (!instanceExtensionsAvailable(enabledExtensionCount, enabledExtensionNames))
        {
            vkDestroyInstance(instance, nullptr);

            return IInstanceSP();
        }
    }

    auto newInstance = new Instance(applicationName, applicationVersion, std::string(applicationInfo.pEngineName), applicationInfo.engineVersion, apiVersion, flags, enabledLayerCount, enabledLayerNames, enabledExtensionCount, enabledExtensionNames, instance);

    if (!newInstance)
    {
        vkDestroyInstance(instance, nullptr);

        return IInstanceSP();
    }

    return IInstanceSP(newInstance);
}

}
