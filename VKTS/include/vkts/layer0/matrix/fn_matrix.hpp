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

#ifndef VKTS_FN_MATRIX_HPP_
#define VKTS_FN_MATRIX_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY translateMat4(const float x, const float y, const float z);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat3 VKTS_APIENTRY translateMat3(const float x, const float y);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY rotateAxisMat4(const float angle, const float x, const float y, const float z);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat3 VKTS_APIENTRY rotateAxisMat3(const float angle, const float x, const float y, const float z);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat2 VKTS_APIENTRY rotateAxisMat2(const float angle);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY rotateRxMat4(const float angle);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat3 VKTS_APIENTRY rotateRxMat3(const float angle);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY rotateRyMat4(const float angle);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat3 VKTS_APIENTRY rotateRyMat3(const float angle);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY rotateRzMat4(const float angle);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat3 VKTS_APIENTRY rotateRzMat3(const float angle);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY rotateRzRxRyMat4(const float anglez, const float anglex, const float angley);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY rotateRzRyRxMat4(const float anglez, const float angley, const float anglex);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat3 VKTS_APIENTRY rotateRzRxRyMat3(const float anglez, const float anglex, const float angley);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat3 VKTS_APIENTRY rotateRzRyRxMat3(const float anglez, const float angley, const float anglex);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY scaleMat4(const float x, const float y, const float z);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat3 VKTS_APIENTRY scaleMat3(const float x, const float y, const float z);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat2 VKTS_APIENTRY scaleMat2(const float x, const float y);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat4 VKTS_APIENTRY shearMat4(const float shxy, const float shxz, const float shyx, const float shyz, const float shzx, const float shzy);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat3 VKTS_APIENTRY shearMat3(const float shxy, const float shxz, const float shyx, const float shyz, const float shzx, const float shzy);

/**
 *
 * @ThreadSafe
 */
VKTS_APICALL glm::mat2 VKTS_APIENTRY shearMat2(const float shx, const float shy);

}

#endif /* VKTS_FN_MATRIX_HPP_ */
