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

#ifndef VKTS_THREADSAFEQUEUE_HPP_
#define VKTS_THREADSAFEQUEUE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

template<class ELEMENT>
class ThreadsafeQueue
{

private:

    mutable std::mutex queueMutex;

    std::condition_variable queueConditionVariable;

    std::queue<ELEMENT> allElements;

public:

    ThreadsafeQueue() :
    	queueMutex(), queueConditionVariable(), allElements()
    {
    }

    ThreadsafeQueue(const ThreadsafeQueue& other)
    {
        std::lock_guard<std::mutex> queueLock(other.queueMutex);

        allElements = other.allElements;
    }

    ThreadsafeQueue(ThreadsafeQueue&& other)
    {
        std::lock_guard<std::mutex> queueLock(other.queueMutex);

        allElements = std::move(other.allElements);
    }

    virtual ~ThreadsafeQueue()
    {
        std::lock_guard<std::mutex> queueLock(queueMutex);

        while (allElements.size() > 0)
        {
            allElements.pop();
        }
    }

    ThreadsafeQueue& operator =(const ThreadsafeQueue& other)
    {
        if (this == &other)
        {
            return *this;
        }

        std::lock_guard<std::mutex> queueLock(other.queueMutex);

        allElements = other.allElements;

        return *this;
    }

    ThreadsafeQueue& operator =(ThreadsafeQueue && other)
    {
        if (this == &other)
        {
            return *this;
        }

        std::lock_guard<std::mutex> queueLock(other.queueMutex);

        allElements = std::move(other.allElements);

        return *this;
    }

    void add(const ELEMENT& command)
    {
        std::lock_guard<std::mutex> queueLock(queueMutex);

        allElements.push(command);

        queueConditionVariable.notify_all();
    }

    VkBool32 take(ELEMENT& result)
    {
        std::lock_guard<std::mutex> queueLock(queueMutex);

        if (!allElements.empty())
        {
            result = allElements.front();
            allElements.pop();

            return VK_TRUE;
        }

        return VK_FALSE;
    }

    void waitAndTake(ELEMENT& result)
    {
        std::unique_lock<std::mutex> queueLock(queueMutex);
        queueConditionVariable.wait(queueLock, [this] {return !allElements.empty();});

        result = allElements.front();
        allElements.pop();
    }

    VkBool32 empty() const
    {
        std::lock_guard<std::mutex> queueLock(queueMutex);

        return allElements.empty();
    }

    std::int32_t size() const
    {
        std::lock_guard<std::mutex> queueLock(queueMutex);

        return allElements.size();
    }

};

} /* namespace vkts */

#endif /* VKTS_THREADSAFEQUEUE_HPP_ */
