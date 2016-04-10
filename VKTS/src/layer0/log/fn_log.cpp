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

#include <vkts/vkts.hpp>

#include "../../fn_internal.hpp"

namespace vkts
{

static const char* VKTS_LOG_STRINGS[] = {"", "ERROR", "WARNING", "INFO", "DEBUG", "SEVERE"};

static std::mutex g_logMutex;

static int32_t g_verbosity;

VkBool32 VKTS_APIENTRY logInit()
{
    return logSetLevel(VKTS_LOG_INFO);
}

VkBool32 VKTS_APIENTRY logSetLevel(const int32_t verbosity)
{
    std::lock_guard<std::mutex> logLockGuard(g_logMutex);

    if (verbosity < VKTS_LOG_NOTHING || verbosity > VKTS_LOG_SEVERE)
    {
        return VK_FALSE;
    }

    g_verbosity = verbosity;

    return VK_TRUE;
}

int32_t VKTS_APIENTRY logGetLevel()
{
    std::lock_guard<std::mutex> logLockGuard(g_logMutex);

    return g_verbosity;
}

void VKTS_APIENTRY logPrint(const int32_t verbosity, const char* format, ...)
{
    std::lock_guard<std::mutex> logLockGuard(g_logMutex);

    if (g_verbosity == VKTS_LOG_NOTHING || verbosity == VKTS_LOG_NOTHING)
    {
        return;
    }

    if (g_verbosity >= verbosity)
    {
        const char* logString = "UNKNOWN";
        char buffer[VKTS_MAX_LOG_CHARS + 1];
        va_list argList;

        if (verbosity > VKTS_LOG_NOTHING && verbosity <= VKTS_LOG_SEVERE)
        {
            logString = VKTS_LOG_STRINGS[verbosity];
        }

        buffer[VKTS_MAX_LOG_CHARS] = '\0';

        va_start(argList, format);

        vsnprintf(buffer, VKTS_MAX_LOG_CHARS, format, argList);

        VKTS_PRINTF("LOG [%s]: %s\n", logString, buffer);

        va_end(argList);
    }
}

void VKTS_APIENTRY logTerminate()
{
    // Nothing for now.
}

}
