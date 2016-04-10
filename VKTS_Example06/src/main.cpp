/**
 * VKTS Examples - Examples for Vulkan using VulKan ToolS.
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

#include "Example.hpp"

static vkts::IInitialResourcesSP initialResources;

static vkts::INativeDisplaySP display;

static vkts::INativeWindowSP window;

static vkts::ISurfaceSP surface;

static void terminateApp()
{
	if (initialResources.get())
	{
		initialResources->destroyDevice();
	}

	//

	if (surface.get())
	{
		surface->destroy();
	}

	//

	if (window.get())
	{
		window->destroy();
	}

	if (display.get())
	{
		display->destroy();
	}

	vkts::visualTerminate();

	//

	if (initialResources.get())
	{
		initialResources->destroyInstance();

		initialResources.reset();
	}

	//

	vkts::engineTerminate();
}

int main()
{
	//
	// Engine initialization.
	//

	if (!vkts::engineInit())
	{
		return -1;
	}

	vkts::logSetLevel(VKTS_LOG_INFO);

	//

	VkResult result;

	//

	if (!vkts::wsiGatherNeededInstanceExtensions())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Example: Could not gather instance extensions.");

		terminateApp();

		return -1;
	}

	auto instance = vkts::instanceCreate(VKTS_EXAMPLE_NAME, VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0), 0, 0, nullptr, vkts::extensionGetNeededInstanceExtensionCount(), vkts::extensionGetNeededInstanceExtensionNames());

	if (!instance.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not create instance.");

		terminateApp();

		return -1;
	}

	if (!vkts::wsiInitInstanceExtensions(instance->getInstance()))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not initialize instance extension.");

		terminateApp();

		return -1;
	}

	auto physicalDevice = vkts::physicalDeviceCreate(instance->getInstance(), 0);

	if (!physicalDevice.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not get physical device.");

		terminateApp();

		return -1;
	}

	//

	VkPhysicalDeviceProperties physicalDeviceProperties;

	physicalDevice->getPhysicalDeviceProperties(physicalDeviceProperties);

	// Check, if uniform buffer size is large enough.
	if (physicalDeviceProperties.limits.maxUniformBufferRange < VKTS_MAX_JOINTS_BUFFERSIZE)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Physical device does not have needed uniform buffer range: %u < %u", physicalDeviceProperties.limits.maxUniformBufferRange, VKTS_MAX_JOINTS_BUFFERSIZE);

		return VK_FALSE;
	}

	//

	if (!vkts::wsiGatherNeededDeviceExtensions(physicalDevice->getPhysicalDevice()))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not gather device extension.");

		terminateApp();

		return -1;
	}

	//
	// Visual initialization.
	//

	if (!vkts::visualInit(instance->getInstance(), physicalDevice->getPhysicalDevice()))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not initialize visual.");

		terminateApp();

		return -1;
	}

	display = vkts::visualCreateDefaultDisplay().lock();

	if (!display.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not create display.");

		terminateApp();

		return -1;
	}

	window = vkts::visualCreateWindow(display, VKTS_EXAMPLE_NAME, 1024, 768, VK_FALSE, VK_TRUE, VK_FALSE).lock();

	if (!window.get())
	{
		window = vkts::visualGetWindow(VKTS_DEFAULT_WINDOW_INDEX).lock();

		if (!window.get())
		{
			vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not create window.");

			terminateApp();

			return -1;
		}
	}

	//

	surface = vkts::wsiSurfaceCreate(instance->getInstance(), display->getNativeDisplay(), window->getNativeWindow());

	if (!surface.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not create surface.");

		terminateApp();

		return -1;
	}

	//

	std::vector<VkBool32> supportFilter;

	result = vkts::wsiGetPhysicalDeviceSurfaceSupport(physicalDevice->getPhysicalDevice(), surface->getSurface(), (uint32_t) physicalDevice->getAllQueueFamilyProperties().size(), supportFilter);

	if (result != VK_SUCCESS || supportFilter.size() == 0)
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not get physical device surface support.");

		terminateApp();

		return -1;
	}

	//

	uint32_t queueFamilyIndex;

	if (!vkts::queueGetFamilyIndex(physicalDevice->getAllQueueFamilyProperties(), VK_QUEUE_GRAPHICS_BIT, 0, &supportFilter, queueFamilyIndex))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not find queue family index.");

		terminateApp();

		return -1;
	}

	//

	float queuePriorities[1] = {0.0f};

	VkDeviceQueueCreateInfo deviceQueueCreateInfo;

	memset(&deviceQueueCreateInfo, 0, sizeof(VkDeviceQueueCreateInfo));

	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

	deviceQueueCreateInfo.flags = 0;
	deviceQueueCreateInfo.queueFamilyIndex = 0;
	deviceQueueCreateInfo.queueCount = 1;
	deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

	auto device = vkts::deviceCreate(physicalDevice->getPhysicalDevice(), 0, 1, &deviceQueueCreateInfo, 0, nullptr, vkts::extensionGetNeededDeviceExtensionCount(), vkts::extensionGetNeededDeviceExtensionNames(), nullptr);

	if (!device.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not create device.");

		terminateApp();

		return -1;
	}

	if (!vkts::wsiInitDeviceExtensions(device->getDevice()))
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not initialize device extension.");

		terminateApp();

		return -1;
	}

	//

	auto queue = vkts::queueGet(device->getDevice(), queueFamilyIndex, 0);

	if (!queue.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not get device queue.");

		terminateApp();

		return -1;
	}

	//

	initialResources = vkts::initialResourcesCreate(instance, physicalDevice, device, queue);

	if (!initialResources.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not create initial resources.");

		terminateApp();

		return -1;
	}

	//
	// Example setup.
	//

	// Single threaded application, so it is safe to pass display and window.
	vkts::IUpdateThreadSP example = vkts::IUpdateThreadSP(new Example(initialResources, window->getIndex(), surface));

	if (!example.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not create application.");

		terminateApp();

		return -1;
	}

	// This update thread is executed in the main loop. No additional thread is launched.
	vkts::engineAddUpdateThread(example);

	//
	// Execution.
	//

	vkts::engineRun();

	//
	// Termination.
	//

	terminateApp();

	return 0;
}
