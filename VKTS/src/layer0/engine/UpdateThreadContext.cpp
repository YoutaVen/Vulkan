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

#include "UpdateThreadContext.hpp"

namespace vkts
{

NativeDisplaySP UpdateThreadContext::getDisplay(int32_t displayIndex) const
{
    size_t index = allAttachedDisplays.find(displayIndex);

    if (index == allAttachedDisplays.size())
    {
        return NativeDisplaySP();
    }

    return allAttachedDisplays.valueAt(index);
}

NativeWindowSP UpdateThreadContext::getWindow(int32_t windowIndex) const
{
    size_t index = allAttachedWindows.find(windowIndex);

    if (index == allAttachedWindows.size())
    {
        return NativeWindowSP();
    }

    return allAttachedWindows.valueAt(index);
}

UpdateThreadContext::UpdateThreadContext(const int32_t threadIndex, const int32_t threadCount, const double tickTime, const TaskQueueSP& taskQueue) :
    IUpdateThreadContext(), threadIndex(threadIndex), threadCount(threadCount), taskQueue(taskQueue), allAttachedDisplays(), allAttachedDisplayKeys(), allAttachedWindows(), allAttachedWindowKeys()
{
    this->startTime = timeGetRaw();
    this->lastTime = startTime;
    this->currentTime = startTime;

    this->tickTime = tickTime;
    this->lastTicks = 0;
    this->currentTicks = 0;
}

UpdateThreadContext::~UpdateThreadContext()
{
    allAttachedDisplays.clear();
    allAttachedDisplayKeys.clear();

    allAttachedWindows.clear();
    allAttachedWindowKeys.clear();
}

void UpdateThreadContext::attachDisplay(const NativeDisplaySP& display)
{
    if (!display.get())
    {
        return;
    }

    allAttachedDisplays[display->getIndex()] = display;

    if (std::find(allAttachedDisplayKeys.begin(), allAttachedDisplayKeys.end(), display->getIndex()) == allAttachedDisplayKeys.end())
    {
        allAttachedDisplayKeys.push_back(display->getIndex());
    }
}

void UpdateThreadContext::attachWindow(const NativeWindowSP& window)
{
    if (!window.get())
    {
        return;
    }

    allAttachedWindows[window->getIndex()] = window;

    if (std::find(allAttachedWindowKeys.begin(), allAttachedWindowKeys.end(), window->getIndex()) == allAttachedWindowKeys.end())
    {
        allAttachedWindowKeys.push_back(window->getIndex());
    }
}

void UpdateThreadContext::update()
{
    lastTime = currentTime;
    currentTime = timeGetRaw();

    lastTicks = currentTicks;
    currentTicks = static_cast<uint64_t>(getTotalTime() / getTickTime());
}

//
// IUpdateThreadContext
//

// Thread functions.

int32_t UpdateThreadContext::getThreadIndex() const
{
    return threadIndex;
}

int32_t UpdateThreadContext::getThreadCount() const
{
    return threadCount;
}

// Time functions.

double UpdateThreadContext::getTotalTime() const
{
    return currentTime - startTime;
}

double UpdateThreadContext::getDeltaTime() const
{
    return currentTime - lastTime;
}

double UpdateThreadContext::getTickTime() const
{
    return tickTime;
}

uint64_t UpdateThreadContext::getTotalTicks() const
{
    return currentTicks;
}

uint64_t UpdateThreadContext::getDeltaTicks() const
{
    return currentTicks - lastTicks;
}

// Display functions.

VkBool32 UpdateThreadContext::isDisplayAttached(const int32_t displayIndex) const
{
    return getDisplay(displayIndex).get() != nullptr;
}

const std::vector<int32_t>& UpdateThreadContext::getAttachedDisplayIndices() const
{
    return allAttachedDisplayKeys;
}

const glm::uvec2& UpdateThreadContext::getDisplayDimension(const int32_t displayIndex) const
{
    static glm::uvec2 noDimension = glm::uvec2(0, 0);

    auto currentDisplay = getDisplay(displayIndex);

    if (currentDisplay.get())
    {
        return currentDisplay->getDimension();
    }

    return noDimension;
}

VKTS_NATIVE_DISPLAY UpdateThreadContext::getNativeDisplay(const int32_t displayIndex) const
{
    static VKTS_NATIVE_DISPLAY noDisplay;

    auto currentDisplay = getDisplay(displayIndex);

    if (currentDisplay.get())
    {
        return currentDisplay->getNativeDisplay();
    }

    return noDisplay;
}

// Window functions.

VkBool32 UpdateThreadContext::isWindowAttached(const int32_t windowIndex) const
{
    return getWindow(windowIndex).get() != nullptr;
}

const std::vector<int32_t>& UpdateThreadContext::getAttachedWindowIndices() const
{
    return allAttachedWindowKeys;
}

const glm::uvec2& UpdateThreadContext::getWindowDimension(const int32_t windowIndex) const
{
    static glm::uvec2 noDimension = glm::uvec2(0, 0);

    auto currentWindow = getWindow(windowIndex);

    if (currentWindow.get())
    {
        return currentWindow->getDimension();
    }

    return noDimension;
}

VKTS_NATIVE_WINDOW UpdateThreadContext::getNativeWindow(const int32_t windowIndex) const
{
    static VKTS_NATIVE_WINDOW noWindow;

    auto currentWindow = getWindow(windowIndex);

    if (currentWindow.get())
    {
        return currentWindow->getNativeWindow();
    }

    return noWindow;
}

// Input functions.

VkBool32 UpdateThreadContext::getKey(const int32_t windowIndex, const int32_t keyIndex) const
{
    auto currentWindow = getWindow(windowIndex);

    if (currentWindow.get())
    {
        return currentWindow->getKeyInput().getKey(keyIndex);
    }

    return VK_FALSE;
}

VkBool32 UpdateThreadContext::getMouseButton(const int32_t windowIndex, const int32_t buttonIndex) const
{
    auto currentWindow = getWindow(windowIndex);

    if (currentWindow.get())
    {
        return currentWindow->getMouseInput().getButton(buttonIndex);
    }

    return VK_FALSE;
}

const glm::ivec2& UpdateThreadContext::getMouseLocation(const int32_t windowIndex) const
{
    static glm::ivec2 noLocation = glm::ivec2(-1.0, -1.0);

    auto currentWindow = getWindow(windowIndex);

    if (currentWindow.get())
    {
        return currentWindow->getMouseInput().getLocation();
    }

    return noLocation;
}

VkBool32 UpdateThreadContext::isGameMouse(const int32_t windowIndex) const
{
    auto currentWindow = getWindow(windowIndex);

    if (currentWindow.get())
    {
        return currentWindow->isInvisibleCursor();
    }

    return VK_FALSE;
}

int32_t UpdateThreadContext::getMouseWheel(const int32_t windowIndex) const
{
    auto currentWindow = getWindow(windowIndex);

    if (currentWindow.get())
    {
        return currentWindow->getMouseInput().getMouseWheel();
    }

    return 0;
}

VkBool32 UpdateThreadContext::getGamepadButton(const int32_t windowIndex, const int32_t gamepadIndex, const int32_t buttonIndex) const
{
    auto currentWindow = getWindow(windowIndex);

    if (currentWindow.get())
    {
        if (gamepadIndex < 0 || gamepadIndex >= VKTS_MAX_GAMEPADS)
        {
            return VK_FALSE;
        }

        return currentWindow->getGamepadInput(gamepadIndex).getButton(buttonIndex);
    }

    return VK_FALSE;
}

float UpdateThreadContext::getGamepadAxis(const int32_t windowIndex, const int32_t gamepadIndex, const int32_t axisIndex) const
{
    auto currentWindow = getWindow(windowIndex);

    if (currentWindow.get())
    {
        if (gamepadIndex < 0 || gamepadIndex >= VKTS_MAX_GAMEPADS)
        {
            return 0.0f;
        }

        return currentWindow->getGamepadInput(gamepadIndex).getAxis(axisIndex);
    }

    return 0.0f;
}

VkBool32 UpdateThreadContext::sendTask(const ITaskSP& task) const
{
    if (!taskQueue.get())
    {
        return VK_FALSE;
    }

    return taskQueue->addTask(task);
}

} /* namespace vkts */
