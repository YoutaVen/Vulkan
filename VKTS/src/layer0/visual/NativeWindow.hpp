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

#ifndef VKTS_NATIVEWINDOW_HPP_
#define VKTS_NATIVEWINDOW_HPP_

#include <vkts/vkts.hpp>

#include "../../fn_internal.hpp"

#include "NativeDisplay.hpp"

namespace vkts
{

class NativeWindow: public INativeWindow, public std::enable_shared_from_this<NativeWindow>
{

private:

    const INativeDisplayWP display;

    VKTS_NATIVE_WINDOW nativeWindow;

    const int32_t index;

    const char* title;

    glm::uvec2 dimension;

    glm::uvec2 tempDimension;

    const VkBool32 fullscreen;

    const VkBool32 resizable;

    const VkBool32 invisibleCursor;

    mutable std::mutex mutex;

    KeyInput keyInput;

    MouseInput mouseInput;

    GamepadInput gamepad[VKTS_MAX_GAMEPADS];

public:

    NativeWindow() = delete;
    NativeWindow(const NativeWindow& other) = delete;
    NativeWindow(NativeWindow&& other) = delete;
    NativeWindow(const INativeDisplayWP& display, VKTS_NATIVE_WINDOW nativeWindow, const int32_t index, const char* title, const uint32_t width, const uint32_t height, const VkBool32 fullscreen, const VkBool32 resizable, const VkBool32 invisibleCursor);
    virtual ~NativeWindow();

    NativeWindow& operator =(const NativeWindow& other) = delete;

    NativeWindow& operator =(NativeWindow && other) = delete;

    const glm::uvec2& getDimension() const;

    void setDimension(const glm::uvec2& dimension);

    const glm::uvec2& getTempDimension() const;

    void setTempDimension(const glm::uvec2& tempDimension);

    KeyInput& getKeyInput();

    const KeyInput& getKeyInput() const;

    MouseInput& getMouseInput();

    const MouseInput& getMouseInput() const;

    GamepadInput& getGamepadInput(const int32_t gamepadIndex);

    const GamepadInput& getGamepadInput(const int32_t gamepadIndex) const;

    //
    // INativeWindow
    //

    virtual const INativeDisplayWP& getNativeDisplay() const override;

    virtual VKTS_NATIVE_WINDOW getNativeWindow() const override;

    virtual int32_t getIndex() const override;

    virtual const char* getTitle() const override;

    virtual uint32_t getWidth() const override;

    virtual uint32_t getHeight() const override;

    virtual VkBool32 isFullscreen() const override;

    virtual VkBool32 isResizable() const override;

    virtual VkBool32 isInvisibleCursor() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

typedef std::shared_ptr<NativeWindow> NativeWindowSP;

} /* namespace vkts */

#endif /* VKTS_NATIVEWINDOW_HPP_ */
