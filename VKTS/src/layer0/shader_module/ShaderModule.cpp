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

#include "../shader_module/ShaderModule.hpp"

namespace vkts
{

ShaderModule::ShaderModule(const VkDevice device, const VkShaderModuleCreateFlags flags, const size_t codeSize, const void* code, const VkShaderModule shaderModule) :
    IShaderModule(), device(device), shaderModuleCreateInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, nullptr, flags, codeSize, nullptr}, shaderModule(shaderModule)
{
    this->code = binaryBufferCreate((uint8_t*) code, codeSize);

    if (this->code.get())
    {
        shaderModuleCreateInfo.pCode = (const uint32_t*) this->code->getData();
    }
    else
    {
        shaderModuleCreateInfo.codeSize = 0;
    }
}

ShaderModule::~ShaderModule()
{
    destroy();
}

//
// IShaderModule
//

const VkDevice ShaderModule::getDevice() const
{
    return device;
}

const VkShaderModuleCreateInfo& ShaderModule::getShaderModuleCreateInfo() const
{
    return shaderModuleCreateInfo;
}

size_t ShaderModule::getCodeSize() const
{
    return shaderModuleCreateInfo.codeSize;
}

const void* ShaderModule::getCode() const
{
    return shaderModuleCreateInfo.pCode;
}

VkShaderModuleCreateFlags ShaderModule::getFlags() const
{
    return shaderModuleCreateInfo.flags;
}

const VkShaderModule ShaderModule::getShaderModule() const
{
    return shaderModule;
}

//
// IDestroyable
//

void ShaderModule::destroy()
{
    if (shaderModule)
    {
        vkDestroyShaderModule(device, shaderModule, nullptr);

        shaderModule = VK_NULL_HANDLE;
    }
}

} /* namespace vkts */
