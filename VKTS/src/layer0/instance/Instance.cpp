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

#include "Instance.hpp"

namespace vkts
{

Instance::Instance(const std::string& applicationName, const uint32_t applicationVersion, const std::string& engineName, const uint32_t engineVersion, const uint32_t apiVersion, const VkInstanceCreateFlags flags, const uint32_t enabledLayerNameCount, const char* const* enabledLayerNames, const uint32_t enabledExtensionNameCount, const char* const* enabledExtensionNames, const VkInstance instance) :
    IInstance(), applicationName(applicationName), engineName(engineName), applicationInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, this->applicationName.c_str(), applicationVersion, this->engineName.c_str(), engineVersion, apiVersion}, instanceCreateInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr, flags, &this->applicationInfo, enabledLayerNameCount, enabledLayerNames, enabledExtensionNameCount, enabledExtensionNames}, instance(instance)
{
}

Instance::~Instance()
{
    destroy();
}

//
// IInstance
//

const VkApplicationInfo& Instance::getApplicationInfo() const
{
    return applicationInfo;
}

const char* Instance::getApplicationName() const
{
    return applicationInfo.pApplicationName;
}

uint32_t Instance::getApplicationVersion() const
{
    return applicationInfo.applicationVersion;
}

const char* Instance::getEngineName() const
{
    return applicationInfo.pEngineName;
}

uint32_t Instance::getEngineVersion() const
{
    return applicationInfo.engineVersion;
}

uint32_t Instance::getApiVersion() const
{
    return applicationInfo.apiVersion;
}

const VkInstanceCreateInfo& Instance::getInstanceCreateInfo() const
{
    return instanceCreateInfo;
}

VkInstanceCreateFlags Instance::getFlags() const
{
    return instanceCreateInfo.flags;
}

uint32_t Instance::getEnabledLayerCount() const
{
    return instanceCreateInfo.enabledLayerCount;
}

const char* const* Instance::getEnabledLayerNames() const
{
    return instanceCreateInfo.ppEnabledLayerNames;
}

uint32_t Instance::getEnabledExtensionCount() const
{
    return instanceCreateInfo.enabledExtensionCount;
}

const char* const* Instance::getEnabledExtensionNames() const
{
    return instanceCreateInfo.ppEnabledExtensionNames;
}

const VkInstance Instance::getInstance() const
{
    return instance;
}

//
// IDestroyable
//

void Instance::destroy()
{
    if (instance)
    {
        vkDestroyInstance(instance, nullptr);

        instance = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
