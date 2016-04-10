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

#include "Semaphore.hpp"

namespace vkts
{

Semaphore::Semaphore(const VkDevice device, const VkSemaphoreCreateFlags flags, const VkSemaphore semaphore) :
    ISemaphore(), device(device), semaphoreCreateInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, nullptr, flags }, semaphore(semaphore)
{
}

Semaphore::~Semaphore()
{
    destroy();
}

//
// ISemaphore
//

const VkDevice Semaphore::getDevice() const
{
    return device;
}

const VkSemaphoreCreateInfo& Semaphore::getSemaphoreCreateInfo() const
{
    return semaphoreCreateInfo;
}

VkSemaphoreCreateFlags Semaphore::getFlags() const
{
    return semaphoreCreateInfo.flags;
}

const VkSemaphore Semaphore::getSemaphore() const
{
    return semaphore;
}

//
// IResetable
//

VkResult Semaphore::reset()
{
    if (semaphore)
    {
        vkDestroySemaphore(device, semaphore, nullptr);

        semaphore = VK_NULL_HANDLE;
    }

    return vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &semaphore);
}

//
// IDestroyable
//

void Semaphore::destroy()
{
    if (semaphore)
    {
        vkDestroySemaphore(device, semaphore, nullptr);

        semaphore = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
