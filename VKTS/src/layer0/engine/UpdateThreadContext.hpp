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

#ifndef VKTS_UPDATETHREADCONTEXT_HPP_
#define VKTS_UPDATETHREADCONTEXT_HPP_

#include <vkts/vkts.hpp>

#include "../../fn_internal.hpp"

#include "TaskQueue.hpp"

namespace vkts
{

class UpdateThreadContext: public IUpdateThreadContext
{

private:

    const int32_t threadIndex;

    const int32_t threadCount;

    double startTime;
    double lastTime;
    double currentTime;

    double tickTime;

    TaskQueueSP taskQueue;

    uint64_t lastTicks;
    uint64_t currentTicks;

    SmartPointerMap<int32_t, NativeDisplaySP> allAttachedDisplays;
    std::vector<int32_t> allAttachedDisplayKeys;

    //

    SmartPointerMap<int32_t, NativeWindowSP> allAttachedWindows;
    std::vector<int32_t> allAttachedWindowKeys;

    //

    NativeDisplaySP getDisplay(int32_t displayIndex) const;

    NativeWindowSP getWindow(int32_t windowIndex) const;

public:

    UpdateThreadContext() = delete;
    UpdateThreadContext(const UpdateThreadContext& other) = delete;
    UpdateThreadContext(UpdateThreadContext&& other) = delete;
    UpdateThreadContext(const int32_t threadIndex, const int32_t threadCount, const double tickTime, const TaskQueueSP& taskQueue);
    virtual ~UpdateThreadContext();

    UpdateThreadContext& operator =(const UpdateThreadContext& other) = delete;
    UpdateThreadContext& operator =(UpdateThreadContext && other) = delete;

    void attachDisplay(const NativeDisplaySP& display);

    void attachWindow(const NativeWindowSP& window);

    void update();

    //
    // IUpdateThreadContext
    //

    // Thread functions.

    virtual int32_t getThreadIndex() const override;

    virtual int32_t getThreadCount() const override;

    // Time functions.

    virtual double getTotalTime() const override;

    virtual double getDeltaTime() const override;

    virtual double getTickTime() const override;

    virtual uint64_t getTotalTicks() const override;

    virtual uint64_t getDeltaTicks() const override;

    // Display functions.

    virtual VkBool32 isDisplayAttached(const int32_t displayIndex) const override;

    virtual const std::vector<int32_t>& getAttachedDisplayIndices() const override;

    virtual const glm::uvec2& getDisplayDimension(const int32_t displayIndex) const override;

    virtual VKTS_NATIVE_DISPLAY getNativeDisplay(const int32_t displayIndex) const override;

    // Window functions.

    virtual VkBool32 isWindowAttached(const int32_t windowIndex) const override;

    virtual const std::vector<int32_t>& getAttachedWindowIndices() const override;

    virtual const glm::uvec2& getWindowDimension(const int32_t windowIndex) const override;

    virtual VKTS_NATIVE_WINDOW getNativeWindow(const int32_t windowIndex) const override;

    // Input functions

    virtual VkBool32 getKey(const int32_t windowIndex, const int32_t keyIndex) const override;

    virtual VkBool32 getMouseButton(const int32_t windowIndex, const int32_t buttonIndex) const override;

    virtual const glm::ivec2& getMouseLocation(const int32_t windowIndex) const override;

    virtual VkBool32 isGameMouse(const int32_t windowIndex) const override;

    virtual int32_t getMouseWheel(const int32_t windowIndex) const override;

    virtual VkBool32 getGamepadButton(const int32_t windowIndex, const int32_t gamepadIndex, const int32_t buttonIndex) const override;

    virtual float getGamepadAxis(const int32_t windowIndex, const int32_t gamepadIndex, const int32_t axisIndex) const override;

    // Task functions

    virtual VkBool32 sendTask(const ITaskSP& task) const override;

};

typedef std::shared_ptr<UpdateThreadContext> UpdateThreadContextSP;

} /* namespace vkts */

#endif /* VKTS_UPDATETHREADCONTEXT_HPP_ */
