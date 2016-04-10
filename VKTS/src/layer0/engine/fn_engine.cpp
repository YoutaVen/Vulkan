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

#include "MessageDispatcher.hpp"
#include "TaskExecutor.hpp"
#include "UpdateThreadExecutor.hpp"

enum VKTS_ENGINE_STATES
{
    VKTS_ENGINE_INIT_STATE, VKTS_ENGINE_UPDATE_STATE
};

namespace vkts
{

typedef std::shared_ptr<std::thread> ThreadSP;

static VKTS_ENGINE_STATES g_engineState = VKTS_ENGINE_INIT_STATE;

static SmartPointerVector<IUpdateThreadSP> g_allUpdateThreads;

static double g_tickTime = 1.0 / VKTS_TICKS_PER_SECOND;

static std::multimap<IUpdateThreadSP, const NativeDisplaySP> g_allAttachedDisplays;

static std::multimap<IUpdateThreadSP, const NativeWindowSP> g_allAttachedWindows;

static uint32_t g_taskExecutorCount = 0;

VkBool32 VKTS_APIENTRY engineInit()
{
    if (!processorInit())
    {
        VKTS_PRINTF("LOG [VKTS_LOG_ERROR]: Engine: Initialization failed! Could not initialize the processor!\n");

        return VK_FALSE;
    }

    if (!logInit())
    {
        VKTS_PRINTF("LOG [VKTS_LOG_ERROR]: Engine: Initialization failed! Could not initialize logging.\n");

        return VK_FALSE;
    }

    if (!timeInit())
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Initialization failed! Could not initialize the timer!");

        return VK_FALSE;
    }

    if (!barrierInit())
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Initialization failed! Could not initialize the barrier!");

        return VK_FALSE;
    }

    return VK_TRUE;
}

VkBool32 VKTS_APIENTRY engineAddUpdateThread(const IUpdateThreadSP& updateThread)
{
    if (g_engineState != VKTS_ENGINE_INIT_STATE)
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Adding update thread failed! Not in initialize state.");

        return VK_FALSE;
    }

    if (!updateThread.get())
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Adding update thread failed! No update thread.");

        return VK_FALSE;
    }

    if (engineGetNumberUpdateThreads() >= VKTS_MAX_UPDATE_THREADS)
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Adding update thread failed! Too many update threads.");

        return VK_FALSE;
    }

    g_allUpdateThreads.append(updateThread);

    return VK_TRUE;
}

VkBool32 VKTS_APIENTRY engineAttachDisplayToUpdateThread(const INativeDisplayWP& display, const IUpdateThreadSP& updateThread)
{
    if (g_engineState != VKTS_ENGINE_INIT_STATE)
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Adding attaching display failed! Not in initialize state.");

        return VK_FALSE;
    }

    if (!updateThread.get())
    {
        logPrint(VKTS_LOG_ERROR, "Engine: No display or update thread.");

        return VK_FALSE;
    }

    auto sharedDisplay = display.lock();

    if (!sharedDisplay.get())
    {
        return VK_FALSE;
    }

    const auto nativeDisplay = visualGetDisplayInternal(sharedDisplay->getIndex());

    if (!nativeDisplay.get())
    {
        return VK_FALSE;
    }

    g_allAttachedDisplays.insert(std::pair<IUpdateThreadSP, const NativeDisplaySP>(updateThread, nativeDisplay));

    return VK_TRUE;
}

VkBool32 VKTS_APIENTRY engineAttachWindowToUpdateThread(const INativeWindowWP& window, const IUpdateThreadSP& updateThread)
{
    if (g_engineState != VKTS_ENGINE_INIT_STATE)
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Adding attaching window failed! Not in initialize state.");

        return VK_FALSE;
    }

    if (!updateThread.get())
    {
        logPrint(VKTS_LOG_ERROR, "Engine: No window or update thread.");

        return VK_FALSE;
    }

    auto sharedWindow = window.lock();

    if (!sharedWindow.get())
    {
        return VK_FALSE;
    }

    const auto nativeWindow = visualGetWindowInternal(sharedWindow->getIndex());

    if (!nativeWindow.get())
    {
        return VK_FALSE;
    }

    g_allAttachedWindows.insert(std::pair<IUpdateThreadSP, const NativeWindowSP>(updateThread, nativeWindow));

    return VK_TRUE;
}

VkBool32 VKTS_APIENTRY engineSetTicksPerSecond(const double ticksPerSecond)
{
    if (g_engineState != VKTS_ENGINE_INIT_STATE)
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Setting ticks failed! Not in initialize state.");

        return VK_FALSE;
    }

    double newTicksPerSecond = glm::clamp(ticksPerSecond, VKTS_TICKS_PER_SECOND_MIN, VKTS_TICKS_PER_SECOND_MAX);

    g_tickTime = 1.0 / newTicksPerSecond;

    return VK_TRUE;
}

VkBool32 VKTS_APIENTRY engineRun()
{
    if (g_engineState != VKTS_ENGINE_INIT_STATE)
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Run failed! Not in initialize state.");

        return VK_FALSE;
    }

    if (engineGetNumberUpdateThreads() < VKTS_MIN_UPDATE_THREADS || engineGetNumberUpdateThreads() > VKTS_MAX_UPDATE_THREADS)
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Run failed! Number of update threads not correct.");

        return VK_FALSE;
    }

    //
    // Main thread gets all displays and windows attached.
    //

    const auto& displayList = _visualGetActiveDisplays();

    for (size_t i = 0; i < displayList.size(); i++)
    {
        engineAttachDisplayToUpdateThread(displayList[i], g_allUpdateThreads[g_allUpdateThreads.size() - 1]);
    }

    const auto& windowList = _visualGetActiveWindows();

    for (size_t i = 0; i < windowList.size(); i++)
    {
        engineAttachWindowToUpdateThread(windowList[i], g_allUpdateThreads[g_allUpdateThreads.size() - 1]);
    }

    //

    g_engineState = VKTS_ENGINE_UPDATE_STATE;

    logPrint(VKTS_LOG_INFO, "Engine: Started.");

    // Task queue creation.

    TaskQueueSP taskQueue;

    if (g_taskExecutorCount > 0)
    {
        taskQueue = TaskQueueSP(new TaskQueue);

        if (!taskQueue.get())
        {
            logPrint(VKTS_LOG_ERROR, "Engine: Run failed! Could not create task queue.");

            return VK_FALSE;
        }
    }

    // Message dispatcher creation.

    MessageDispatcherSP messageDispatcher = MessageDispatcherSP(new MessageDispatcher());

    if (!messageDispatcher.get())
    {
        logPrint(VKTS_LOG_ERROR, "Engine: Run failed! Could not create message dispatcher.");

        return VK_FALSE;
    }

    // Object, needed for synchronizing the executors.

    ExecutorSync executorSync;

    //
    // Task executor creation and launching,
    //

    SmartPointerVector<TaskExecutorSP> realTaskExecutors;
    SmartPointerVector<ThreadSP> realTaskThreads;

    for (uint32_t i = 0; i < g_taskExecutorCount; i++)
    {
        auto currentTaskExecutor = TaskExecutorSP(new TaskExecutor(i, executorSync, taskQueue));

        if (!currentTaskExecutor.get())
        {
            logPrint(VKTS_LOG_ERROR, "Engine: Run failed! Could not create current task executor.");

            return VK_FALSE;
        }

        auto currentRealThread = ThreadSP(new std::thread(&TaskExecutor::run, currentTaskExecutor));

        if (!currentRealThread.get())
        {
            logPrint(VKTS_LOG_ERROR, "Engine: Run failed! Could not create current real thread.");

            return VK_FALSE;
        }

        //

        realTaskExecutors.append(currentTaskExecutor);
        realTaskThreads.append(currentRealThread);

        logPrint(VKTS_LOG_INFO, "Engine: Task %d started.", currentTaskExecutor->getIndex());
    }

    //
    // Update Thread creation and launching.
    //

    UpdateThreadExecutorSP mainUpdateThreadExecutor;

    SmartPointerVector<UpdateThreadExecutorSP> realUpdateThreadExecutors;
    SmartPointerVector<ThreadSP> realUpdateThreads;

    int32_t index = 0;

    for (size_t updateThreadIndex = 0; updateThreadIndex < g_allUpdateThreads.size(); updateThreadIndex++)
    {
        const auto& currentUpdateThread = g_allUpdateThreads[updateThreadIndex];

        //

        const auto currentMessageDispatcher = (index == engineGetNumberUpdateThreads() - 1) ? messageDispatcher : MessageDispatcherSP();

        //

        auto currentUpdateThreadContext = UpdateThreadContextSP(new UpdateThreadContext((int32_t) updateThreadIndex, (int32_t) g_allUpdateThreads.size(), g_tickTime, taskQueue));

        if (!currentUpdateThreadContext.get())
        {
            logPrint(VKTS_LOG_ERROR, "Engine: Run failed! Could not create update thread context.");

            return VK_FALSE;
        }

        //

        for (auto currentDisplayWalker = g_allAttachedDisplays.lower_bound(currentUpdateThread); currentDisplayWalker != g_allAttachedDisplays.upper_bound(currentUpdateThread); currentDisplayWalker++)
        {
            currentUpdateThreadContext->attachDisplay(currentDisplayWalker->second);
        }

        //

        for (auto currentWindowWalker = g_allAttachedWindows.lower_bound(currentUpdateThread); currentWindowWalker != g_allAttachedWindows.upper_bound(currentUpdateThread); currentWindowWalker++)
        {
            currentUpdateThreadContext->attachWindow(currentWindowWalker->second);
        }

        //

        if (index == engineGetNumberUpdateThreads() - 1)
        {
            // Last thread is the main thread.
            mainUpdateThreadExecutor = UpdateThreadExecutorSP(new UpdateThreadExecutor(index, executorSync, currentUpdateThread, currentUpdateThreadContext, currentMessageDispatcher));

            if (!mainUpdateThreadExecutor.get())
            {
                logPrint(VKTS_LOG_ERROR, "Engine: Run failed! Could not create main update thread executor.");

                return VK_FALSE;
            }
        }
        else
        {
            // Receive queue is the threads send queue.
            auto currentUpdateThreadExecutor = UpdateThreadExecutorSP(new UpdateThreadExecutor(index, executorSync, currentUpdateThread, currentUpdateThreadContext, currentMessageDispatcher));

            if (!currentUpdateThreadExecutor.get())
            {
                logPrint(VKTS_LOG_ERROR, "Engine: Run failed! Could not create current update thread executor.");

                return VK_FALSE;
            }

            realUpdateThreadExecutors.append(currentUpdateThreadExecutor);

            logPrint(VKTS_LOG_INFO, "Engine: Thread %d started.", currentUpdateThreadExecutor->getIndex());

            auto currentRealThread = ThreadSP(new std::thread(&UpdateThreadExecutor::run, currentUpdateThreadExecutor));

            if (!currentRealThread.get())
            {
                logPrint(VKTS_LOG_ERROR, "Engine: Run failed! Could not create current real thread.");

                return VK_FALSE;
            }
            realUpdateThreads.append(currentRealThread);
        }
        index++;
    }

    // Run last thread and loop.
    logPrint(VKTS_LOG_INFO, "Engine: Thread %d started.", mainUpdateThreadExecutor->getIndex());

    mainUpdateThreadExecutor->run();

    //
    // Stopping everything.
    //

    logPrint(VKTS_LOG_INFO, "Engine: Thread %d stopped.", mainUpdateThreadExecutor->getIndex());

    // Wait for all threads to finish in the reverse order they were created.
    for (auto reverseIndex = static_cast<int32_t>(realUpdateThreads.size()) - 1; reverseIndex >= 0; reverseIndex--)
    {
        const auto& currentRealThread = realUpdateThreads[reverseIndex];

        currentRealThread->join();

        logPrint(VKTS_LOG_INFO, "Engine: Thread %d stopped.", reverseIndex);
    }

    realUpdateThreadExecutors.clear();
    realUpdateThreads.clear();

    //

    if (taskQueue.get())
    {
    	// Empty the queue.
    	// As no update thread can feed the queue anymore, it is save to call reset.

    	taskQueue->reset();

    	//

    	ITaskSP stopTask;

        logPrint(VKTS_LOG_SEVERE, "Engine: Disabling task queue.");

    	for (uint32_t i = 0; i < g_taskExecutorCount; i++)
    	{
    		// Send an empty task to the queue, to exit the thread.
    		taskQueue->addTask(stopTask);
    	}
    }

    // Wait for all tasks to finish in the reverse order they were created.
    for (auto reverseIndex = static_cast<int32_t>(realTaskThreads.size()) - 1; reverseIndex >= 0; reverseIndex--)
    {
        const auto& currentRealThread = realTaskThreads[reverseIndex];

        currentRealThread->join();

        logPrint(VKTS_LOG_INFO, "Engine: Task %d stopped.", reverseIndex);
    }

    realTaskExecutors.clear();
    realTaskThreads.clear();

    //

    g_engineState = VKTS_ENGINE_INIT_STATE;

    logPrint(VKTS_LOG_INFO, "Engine: Stopped.");

    return VK_TRUE;
}

int32_t VKTS_APIENTRY engineGetNumberUpdateThreads()
{
    return static_cast<int32_t>(g_allUpdateThreads.size());
}

VkBool32 VKTS_APIENTRY engineSetTaskExecutorCount(const uint32_t count)
{
    g_taskExecutorCount = count;

    return VK_TRUE;
}

uint32_t VKTS_APIENTRY engineGetTaskExecutorCount()
{
    return g_taskExecutorCount;
}

void VKTS_APIENTRY engineTerminate()
{
    barrierTerminate();

    timeTerminate();

    logTerminate();

    processorTerminate();
}

}
