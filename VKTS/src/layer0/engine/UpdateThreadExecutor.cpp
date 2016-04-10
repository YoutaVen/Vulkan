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

#include "UpdateThreadExecutor.hpp"

namespace vkts
{

UpdateThreadExecutor::UpdateThreadExecutor(const int32_t index, ExecutorSync& executorSync, const IUpdateThreadSP& updateThread, const UpdateThreadContextSP& updateThreadContext, const MessageDispatcherSP& messageDispatcher) :
    index(index), executorSync(executorSync), updateThread(updateThread), updateThreadContext(updateThreadContext), messageDispatcher(messageDispatcher)
{
}

UpdateThreadExecutor::~UpdateThreadExecutor()
{
}

int32_t UpdateThreadExecutor::getIndex() const
{
    return index;
}

void UpdateThreadExecutor::run() const
{
    // Initialization.

    VkBool32 doRun = VK_TRUE;

    if (!updateThread->init(*updateThreadContext))
    {
        logPrint(VKTS_LOG_ERROR, "UpdateThreadExecutor %d: Run failed! Update thread Initialize failed.", index);

        doRun = VK_FALSE;
        executorSync.setDoAllRunFalse();

        logPrint(VKTS_LOG_SEVERE, "UpdateThreadExecutor %d: Killing barrier.", index);

        barrierKill();
    }
    else
    {
        logPrint(VKTS_LOG_SEVERE, "UpdateThreadExecutor %d: Initialized.", index);
    }

    // Update loop.

    while (doRun && executorSync.doAllRun())
    {
        doRun = updateThread->update(*updateThreadContext);

        if (!doRun)
        {
            logPrint(VKTS_LOG_INFO, "UpdateThreadExecutor %d: Update ended.", index);

            doRun = VK_FALSE;
            executorSync.setDoAllRunFalse();

            logPrint(VKTS_LOG_SEVERE, "UpdateThreadExecutor %d: Killing barrier.", index);

            barrierKill();

            logPrint(VKTS_LOG_SEVERE, "UpdateThreadExecutor %d: Disabling task queue.", index);

            updateThreadContext->sendTask(ITaskSP());

            break;
        }

        updateThreadContext->update();

        if (messageDispatcher.get())
        {
            if (!messageDispatcher->update())
            {
                logPrint(VKTS_LOG_INFO, "UpdateThreadExecutor %d: Received quit message.", index);

                doRun = VK_FALSE;
                executorSync.setDoAllRunFalse();

                logPrint(VKTS_LOG_SEVERE, "UpdateThreadExecutor %d: Killing barrier.", index);

                barrierKill();

                logPrint(VKTS_LOG_SEVERE, "UpdateThreadExecutor %d: Disabling task queue.", index);

                updateThreadContext->sendTask(ITaskSP(nullptr));

                break;
            }
        }

        std::this_thread::yield();
    }

    // Blocking call, that all executors have finished their update thread.

    executorSync.updateDone();

    // Termination.

    updateThread->terminate(*updateThreadContext);

    logPrint(VKTS_LOG_SEVERE, "UpdateThreadExecutor %d: Terminated.", index);
}

} /* namespace vkts */
