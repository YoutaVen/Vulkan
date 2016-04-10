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

#ifndef VKTS_FN_VISUAL_LINUX_INTERNAL_HPP_
#define VKTS_FN_VISUAL_LINUX_INTERNAL_HPP_

#include <vkts/vkts.hpp>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

namespace vkts
{

VKTS_APICALL VkBool32 VKTS_APIENTRY _visualInitKey();

VKTS_APICALL int32_t VKTS_APIENTRY _visualTranslateKey(int32_t keyCode);

//

VKTS_APICALL VkBool32 VKTS_APIENTRY _visualInitGamepad(const VkInstance instance, const VkPhysicalDevice physicalDevice);

VKTS_APICALL VkBool32 VKTS_APIENTRY _visualDispatchMessagesGamepad();

VKTS_APICALL void VKTS_APIENTRY _visualTerminateGamepad();

//

VKTS_APICALL void VKTS_APIENTRY _visualKeySet(const int32_t keyIndex, const VkBool32 pressed);


VKTS_APICALL void VKTS_APIENTRY _visualMouseSetButton(const int32_t buttonIndex, const VkBool32 pressed);

VKTS_APICALL void VKTS_APIENTRY _visualMouseSetLocation(const glm::ivec2& location);

VKTS_APICALL void VKTS_APIENTRY _visualMouseSetMouseWheel(const int32_t mosueWheel);


VKTS_APICALL void VKTS_APIENTRY _visualGamepadSetButton(const int32_t gamepadIndex, const int32_t buttonIndex, const VkBool32 pressed);

VKTS_APICALL void VKTS_APIENTRY _visualGamepadSetAxis(const int32_t gamepadIndex, const int32_t axisIndex, const float value);

}

#endif /* VKTS_FN_VISUAL_LINUX_INTERNAL_HPP_ */
