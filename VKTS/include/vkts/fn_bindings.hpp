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

#ifndef VKTS_BINDINGS_HPP_
#define VKTS_BINDINGS_HPP_

#include <vkts/vkts.hpp>

#define VKTS_MAX_JOINTS 96

#define VKTS_MAX_JOINTS_BUFFERSIZE (VKTS_MAX_JOINTS * sizeof(float) * ((16 + 1) + (12 + 1)))

// Vertex shader bindings.

#define VKTS_BINDING_UNIFORM_BUFFER_VERTEX_VIEWPROJECTION 0
#define VKTS_BINDING_UNIFORM_BUFFER_VERTEX_TRANSFORM 1
#define VKTS_BINDING_UNIFORM_BUFFER_VERTEX_BONE_TRANSFORM 11

// Fragment shader bindings.

#define VKTS_BINDING_UNIFORM_BUFFER_FRAGMENT_LIGHT 2

#define VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE 3
#define VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_ALPHA 4
#define VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_DISPLACEMENT 5
#define VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_NORMAL 6

#define VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_AMBIENT 7
#define VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_DIFFUSE 8
#define VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_SPECULAR 9
#define VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_SPECULAR_SHININESS 10

// Amount of colors both for Phong shading.

#define VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_COLOR_COUNT 8

// Amount of maximum bindings.

#define VKTS_BINDING_UNIFORM_BINDING_COUNT 12

#endif /* VKTS_BINDINGS_HPP_ */
