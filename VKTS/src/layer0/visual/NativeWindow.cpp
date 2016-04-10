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

#include "fn_visual_internal.hpp"

#include "NativeWindow.hpp"

namespace vkts
{

NativeWindow::NativeWindow(const INativeDisplayWP& display, VKTS_NATIVE_WINDOW nativeWindow, const int32_t index, const char* title, const uint32_t width, const uint32_t height, const VkBool32 fullscreen, const VkBool32 resizable, const VkBool32 invisibleCursor) :
    INativeWindow(), display(display), nativeWindow(nativeWindow), index(index), title(title), dimension(width, height), tempDimension(width, height), fullscreen(fullscreen), resizable(resizable), invisibleCursor(invisibleCursor), mutex(), keyInput(), mouseInput()
{
}

NativeWindow::~NativeWindow()
{
    // No automatic destroy by purpose.
}

const glm::uvec2& NativeWindow::getDimension() const
{
    std::lock_guard<std::mutex> lockGuard(mutex);

    return dimension;
}

void NativeWindow::setDimension(const glm::uvec2& dimension)
{
    std::lock_guard<std::mutex> lockGuard(mutex);

    this->dimension = dimension;
}

const glm::uvec2& NativeWindow::getTempDimension() const
{
    std::lock_guard<std::mutex> lockGuard(mutex);

    return tempDimension;
}

void NativeWindow::setTempDimension(const glm::uvec2& tempDimension)
{
    std::lock_guard<std::mutex> lockGuard(mutex);

    this->tempDimension = tempDimension;
}

KeyInput& NativeWindow::getKeyInput()
{
    return keyInput;
}

const KeyInput& NativeWindow::getKeyInput() const
{
    return keyInput;
}

MouseInput& NativeWindow::getMouseInput()
{
    return mouseInput;
}

const MouseInput& NativeWindow::getMouseInput() const
{
    return mouseInput;
}

GamepadInput& NativeWindow::getGamepadInput(const int32_t gamepadIndex)
{
    // Checked before access.
    assert(gamepadIndex >= 0 && gamepadIndex < VKTS_MAX_GAMEPADS);

    return gamepad[gamepadIndex];
}

const GamepadInput& NativeWindow::getGamepadInput(const int32_t gamepadIndex) const
{
    // Checked before access.
    assert(gamepadIndex >= 0 && gamepadIndex < VKTS_MAX_GAMEPADS);

    return gamepad[gamepadIndex];
}

//
// INativeWindow
//

const INativeDisplayWP& NativeWindow::getNativeDisplay() const
{
    return display;
}

VKTS_NATIVE_WINDOW NativeWindow::getNativeWindow() const
{
    return nativeWindow;
}

int32_t NativeWindow::getIndex() const
{
    return index;
}

const char* NativeWindow::getTitle() const
{
    return title;
}

uint32_t NativeWindow::getWidth() const
{
    return dimension.x;
}

uint32_t NativeWindow::getHeight() const
{
    return dimension.y;
}

VkBool32 NativeWindow::isFullscreen() const
{
    return fullscreen;
}

VkBool32 NativeWindow::isResizable() const
{
    return resizable;
}

VkBool32 NativeWindow::isInvisibleCursor() const
{
    return invisibleCursor;
}

//
// IDestroyable
//

void NativeWindow::destroy()
{
    visualDestroyWindow(shared_from_this());
}

} /* namespace vkts */
