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

#ifndef VKTS_FN_QUAT_HPP_
#define VKTS_FN_QUAT_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL float VKTS_APIENTRY norm(const quat& q);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL quat VKTS_APIENTRY normalize(const quat& q);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL quat VKTS_APIENTRY conjugate(const quat& q);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL quat VKTS_APIENTRY inverse(const quat& q);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL quat VKTS_APIENTRY slerp(const quat& q0, const quat& q1, const float t);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL quat VKTS_APIENTRY rotateAxis(const float angle, const float x, const float y, const float z);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL quat VKTS_APIENTRY rotateRx(const float anglex);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL quat VKTS_APIENTRY rotateRy(const float angley);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL quat VKTS_APIENTRY rotateRz(const float anglez);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL quat VKTS_APIENTRY rotateRzRyRx(const float anglez, const float angley, const float anglex);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL quat VKTS_APIENTRY rotateRzRxRy(const float anglez, const float anglex, const float angley);

}

#endif /* VKTS_FN_QUAT_HPP_ */
