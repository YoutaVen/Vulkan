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

static vkts::IInstanceSP instance;

static vkts::IPhysicalDeviceSP physicalDevice;

static vkts::IDeviceSP device;

static vkts::IQueueSP queue;

static void terminateApp()
{
	if (device.get())
	{
		queue.reset();

		//

		device->destroy();
	}

	//

	physicalDevice.reset();

	if (instance.get())
	{
		instance->destroy();
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

	instance = vkts::instanceCreate(VKTS_EXAMPLE_NAME, VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0), 0, 0, nullptr, 0, nullptr);

	if (!instance.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not create instance.");

		terminateApp();

		return -1;
	}

	physicalDevice = vkts::physicalDeviceCreate(instance->getInstance(), 0);

	if (!physicalDevice.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not get physical device.");

		terminateApp();

		return -1;
	}

	//

	uint32_t queueFamilyIndex;

	if (!vkts::queueGetFamilyIndex(physicalDevice->getAllQueueFamilyProperties(), VK_QUEUE_COMPUTE_BIT, 0, nullptr, queueFamilyIndex))
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

	device = vkts::deviceCreate(physicalDevice->getPhysicalDevice(), 0, 1, &deviceQueueCreateInfo, 0, nullptr, 0, nullptr, nullptr);

	if (!device.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not create device.");

		terminateApp();

		return -1;
	}

	//

	queue = vkts::queueGet(device->getDevice(), queueFamilyIndex, 0);

	if (!queue.get())
	{
		vkts::logPrint(VKTS_LOG_ERROR, "Main: Could not get device queue.");

		terminateApp();

		return -1;
	}

	//
	// Example setup.
	//

	vkts::IUpdateThreadSP example = vkts::IUpdateThreadSP(new Example(instance, physicalDevice, device, queue));

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
