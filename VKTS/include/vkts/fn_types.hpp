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

#ifndef VKTS_TYPES_HPP_
#define VKTS_TYPES_HPP_

#include <vkts/vkts.hpp>

typedef enum VkTsSearch_
{
    VKTS_SEARCH_ABSOLUTE = 0, VKTS_SEARCH_RELATVE = 1
} VkTsSearch;

typedef enum VkTsTargetTransform_
{
    VKTS_TARGET_TRANSFORM_TRANSLATE = 0,
    VKTS_TARGET_TRANSFORM_ROTATE = 1,
    VKTS_TARGET_TRANSFORM_QUATERNION_ROTATE = 2,
    VKTS_TARGET_TRANSFORM_SCALE = 3
} VkTsTargetTransform;

typedef enum VkTsTargetTransformElement_
{
    VKTS_TARGET_TRANSFORM_ELEMENT_X = 0,
    VKTS_TARGET_TRANSFORM_ELEMENT_Y = 1,
    VKTS_TARGET_TRANSFORM_ELEMENT_Z = 2,
	VKTS_TARGET_TRANSFORM_ELEMENT_W = 3
} VkTsTargetTransformElement;

typedef enum VkTsInterpolator_
{
    VKTS_INTERPOLATOR_CONSTANT = 0,
    VKTS_INTERPOLATOR_LINEAR = 1,
    VKTS_INTERPOLATOR_BEZIER = 2
} VkTsInterpolator;

typedef enum VkTsVertexBufferTypeBits_
{
    VKTS_VERTEX_BUFFER_TYPE_VERTEX = 0x00000001,
    VKTS_VERTEX_BUFFER_TYPE_NORMAL = 0x00000002,
    VKTS_VERTEX_BUFFER_TYPE_BITANGENT = 0x00000004,
    VKTS_VERTEX_BUFFER_TYPE_TANGENT = 0x00000008,
    VKTS_VERTEX_BUFFER_TYPE_TEXCOORD = 0x00000010,
    VKTS_VERTEX_BUFFER_TYPE_BONE_INDICES0 = 0x00000020,
    VKTS_VERTEX_BUFFER_TYPE_BONE_INDICES1 = 0x00000040,
    VKTS_VERTEX_BUFFER_TYPE_BONE_WEIGHTS0 = 0x00000080,
    VKTS_VERTEX_BUFFER_TYPE_BONE_WEIGHTS1 = 0x00000100,
    VKTS_VERTEX_BUFFER_TYPE_BONE_NUMBERS = 0x00000200,

    VKTS_VERTEX_BUFFER_TYPE_TANGENTS = VKTS_VERTEX_BUFFER_TYPE_NORMAL | VKTS_VERTEX_BUFFER_TYPE_BITANGENT | VKTS_VERTEX_BUFFER_TYPE_TANGENT,

    VKTS_VERTEX_BUFFER_TYPE_BONES = VKTS_VERTEX_BUFFER_TYPE_BONE_INDICES0 | VKTS_VERTEX_BUFFER_TYPE_BONE_INDICES1 | VKTS_VERTEX_BUFFER_TYPE_BONE_WEIGHTS0 | VKTS_VERTEX_BUFFER_TYPE_BONE_WEIGHTS1 | VKTS_VERTEX_BUFFER_TYPE_BONE_NUMBERS
} VkTsVertexBufferTypeBits;

typedef VkFlags VkTsVertexBufferType;

typedef struct VkTsStructureTypeHeader_
{
    VkStructureType sType;
    const void* pNext;
} VkTsStructureTypeHeader;

#endif /* VKTS_TYPES_HPP_ */
