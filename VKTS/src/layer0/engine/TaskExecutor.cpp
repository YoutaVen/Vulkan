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

#include "TaskExecutor.hpp"

namespace vkts
{

TaskExecutor::TaskExecutor(const int32_t index, ExecutorSync& sync, const TaskQueueSP& taskQueue) :
    index(index), sync(sync), taskQueue(taskQueue)
{
}

TaskExecutor::~TaskExecutor()
{
}

int32_t TaskExecutor::getIndex() const
{
    return index;
}

void TaskExecutor::run() const
{
    logPrint(VKTS_LOG_SEVERE, "TaskExecutor %d: Started.", index);

    ITaskSP task;

    auto doRun = VK_TRUE;

    while (doRun && sync.doAllRun())
    {
        doRun = taskQueue->receiveTask(task);

        if (!task.get())
        {
            doRun = VK_FALSE;
        }

        if (doRun && task.get())
        {
            doRun = task->run();
        }

        if (doRun)
        {
            task = ITaskSP();
        }
        else
        {
            sync.setDoAllRunFalse();
        }

        std::this_thread::yield();
    }

    logPrint(VKTS_LOG_SEVERE, "TaskExecutor %d: Terminated.", index);
}

} /* namespace vkts */
