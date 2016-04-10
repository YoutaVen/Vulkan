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

#ifndef VKTS_FN_INTERNAL_HPP_
#define VKTS_FN_INTERNAL_HPP_

#include <vkts/vkts.hpp>

/**
 * Defines.
 */

#define VKTS_MAX_LOG_CHARS 2048

#define VKTS_MAX_BUFFER_CHARS 2048
#define VKTS_MAX_TOKEN_CHARS 256

#define VKTS_MIN_UPDATE_THREADS 1
#define VKTS_MAX_UPDATE_THREADS 256

#define VKTS_TICKS_PER_SECOND 20.0
#define VKTS_TICKS_PER_SECOND_MIN 1.0
#define VKTS_TICKS_PER_SECOND_MAX 480.0

#define VKTS_BEZIER_TOLERANCE 0.1f
#define VKTS_BEZIER_LOOPS 10

/**
 * Functions.
 */

//
// Extension.
//

#include "layer0/extension/fn_extension_internal.hpp"

//
// Processor.
//
#include "layer0/processor/fn_processor_internal.hpp"

//
// Logging.
//

#include "layer0/log/fn_log_internal.hpp"

//
// Time.
//

#include "layer0/time/fn_time_internal.hpp"

//
// Barrier.
//

#include "layer0/barrier/fn_barrier_internal.hpp"

//
// Input.
//

#include "layer0/input/fn_input_internal.hpp"

//
// File.
//

#include "layer0/file/fn_file_internal.hpp"

//
// Visual.
//

#include "layer0/visual/fn_visual_internal.hpp"

#endif /* VKTS_FN_INTERNAL_HPP_ */
