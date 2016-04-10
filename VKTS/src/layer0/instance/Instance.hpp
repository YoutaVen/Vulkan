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

#ifndef VKTS_INSTANCE_HPP_
#define VKTS_INSTANCE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class Instance: public IInstance
{

private:

    const std::string applicationName;

    const std::string engineName;

    const VkApplicationInfo applicationInfo;

    const VkInstanceCreateInfo instanceCreateInfo;

    VkInstance instance;

public:

    Instance() = delete;
    Instance(const std::string& applicationName, const uint32_t applicationVersion, const std::string& engineName, const uint32_t engineVersion, const uint32_t apiVersion, const VkInstanceCreateFlags flags, const uint32_t enabledLayerNameCount, const char* const* enabledLayerNames, const uint32_t enabledExtensionNameCount, const char* const* enabledExtensionNames, const VkInstance instance);
    Instance(const Instance& other) = delete;
    Instance(Instance&& other) = delete;
    virtual ~Instance();

    Instance& operator =(const Instance& other) = delete;

    Instance& operator =(Instance && other) = delete;

    //
    // IInstance
    //

    virtual const VkApplicationInfo& getApplicationInfo() const override;

    virtual const char* getApplicationName() const override;

    virtual uint32_t getApplicationVersion() const override;

    virtual const char* getEngineName() const override;

    virtual uint32_t getEngineVersion() const override;

    virtual uint32_t getApiVersion() const override;

    virtual const VkInstanceCreateInfo& getInstanceCreateInfo() const override;

    virtual VkInstanceCreateFlags getFlags() const override;

    // ApplicationInfo, see above

    virtual uint32_t getEnabledLayerCount() const override;

    virtual const char* const* getEnabledLayerNames() const override;

    virtual uint32_t getEnabledExtensionCount() const override;

    virtual const char* const* getEnabledExtensionNames() const override;

    virtual const VkInstance getInstance() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_INSTANCE_HPP_ */
