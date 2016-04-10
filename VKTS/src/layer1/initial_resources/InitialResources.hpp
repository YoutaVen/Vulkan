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

#ifndef VKTS_INITIALRESOURCES_HPP_
#define VKTS_INITIALRESOURCES_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class InitialResources: public IInitialResources
{

private:

    IInstanceSP instance;

    IPhysicalDeviceSP physicalDevice;

    IDeviceSP device;

    IQueueSP queue;

public:

    InitialResources() = delete;
    InitialResources(const IInstanceSP& instance, const IPhysicalDeviceSP& physicalDevice, const IDeviceSP& device, const IQueueSP& queue);
    InitialResources(const InitialResources& other) = delete;
    InitialResources(InitialResources&& other) = delete;
    virtual ~InitialResources();

    InitialResources& operator =(const InitialResources& other) = delete;
    InitialResources& operator =(InitialResources && other) = delete;

    //
    // IInitialResources
    //

    virtual const IInstanceSP& getInstance() const override;

    virtual const IPhysicalDeviceSP& getPhysicalDevice() const override;

    virtual void destroyInstance() override;

    virtual const IDeviceSP& getDevice() const override;

    virtual const IQueueSP& getQueue() const override;

    virtual void destroyDevice() override;

    //
    // IDeytroyable
    //

    virtual void destroy() override;
};

} /* namespace vkts */

#endif /* VKTS_INITIALRESOURCES_HPP_ */
