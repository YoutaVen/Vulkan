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

static std::mutex g_barrierMutex;

static std::condition_variable g_barrierConditionVariable;

static int32_t g_barrierExecutingUpdateThreads = 0;

static int32_t g_barrierGeneration = 0;

VkBool32 VKTS_APIENTRY barrierInit()
{
    g_barrierExecutingUpdateThreads = 0;

    g_barrierGeneration = 0;

    return VK_TRUE;
}

VkBool32 VKTS_APIENTRY barrierSync()
{
    std::unique_lock<std::mutex> barrierUniqueLock(g_barrierMutex);

    if (g_barrierGeneration < 0)
    {
        return VK_FALSE;
    }

    // If there is no executing thread, start new barrier ...
    if (g_barrierExecutingUpdateThreads == 0)
    {
        // ... by setting the total number of threads.
        g_barrierExecutingUpdateThreads = engineGetNumberUpdateThreads();

        // Error case, so just return.
        if (g_barrierExecutingUpdateThreads < VKTS_MIN_UPDATE_THREADS)
        {
            return VK_FALSE;
        }
    }

    // Remove this executing thread.
    g_barrierExecutingUpdateThreads--;

    // Are there any executing threads?
    if (g_barrierExecutingUpdateThreads > 0)
    {
        int32_t currentBarrierGeneration = g_barrierGeneration;

        // Yes, so wait until all have reached the barrier.
        do
        {
            g_barrierConditionVariable.wait(barrierUniqueLock);
        }
        while (currentBarrierGeneration == g_barrierGeneration);
    }
    else
    {
        // No, so all have reached the barrier.

        // Initiate next barrier generation. This allows the other threads to proceed.
        g_barrierGeneration++;
        if (g_barrierGeneration < 0)
        {
            g_barrierGeneration = 0;
        }

        // Wake up the rest.
        g_barrierConditionVariable.notify_all();

        return VK_TRUE;
    }

    return VK_TRUE;
}

void VKTS_APIENTRY barrierKill()
{
    std::lock_guard<std::mutex> barrierLockGuard(g_barrierMutex);

    g_barrierExecutingUpdateThreads = 0;

    g_barrierGeneration = -1;

    g_barrierConditionVariable.notify_all();
}

void VKTS_APIENTRY barrierTerminate()
{
    // Nothing for now.
}

}
