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

// 1024 extensions in parallel are possible.
#define VKTS_MAX_EXTENSIONS 1024

namespace vkts
{

static const char* g_instanceExtensionNames[VKTS_MAX_EXTENSIONS];
static uint32_t g_instanceExtensionCount = 0;

uint32_t VKTS_APIENTRY extensionGetNeededInstanceExtensionCount()
{
    return g_instanceExtensionCount;
}

const char** VKTS_APIENTRY extensionGetNeededInstanceExtensionNames()
{
    return g_instanceExtensionNames;
}

VkBool32 VKTS_APIENTRY _extensionAddInstanceExtensions(const char* extension)
{
	if (g_instanceExtensionCount < VKTS_MAX_EXTENSIONS)
	{
		g_instanceExtensionNames[g_instanceExtensionCount] = extension;
		g_instanceExtensionCount++;

		return VK_TRUE;
	}

	return VK_FALSE;
}

//

static const char* g_deviceExtensionNames[VKTS_MAX_EXTENSIONS];
static uint32_t g_deviceExtensionCount = 0;

uint32_t VKTS_APIENTRY extensionGetNeededDeviceExtensionCount()
{
    return g_deviceExtensionCount;
}

const char** VKTS_APIENTRY extensionGetNeededDeviceExtensionNames()
{
    return g_deviceExtensionNames;
}

VkBool32 VKTS_APIENTRY _extensionAddDeviceExtensions(const char* extension)
{
	if (g_deviceExtensionCount < VKTS_MAX_EXTENSIONS)
	{
		g_deviceExtensionNames[g_deviceExtensionCount] = extension;
		g_deviceExtensionCount++;

		return VK_TRUE;
	}

	return VK_FALSE;
}

}
