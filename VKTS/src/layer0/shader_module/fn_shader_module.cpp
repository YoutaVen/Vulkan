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

#include "../shader_module/ShaderModule.hpp"

namespace vkts
{

IShaderModuleSP VKTS_APIENTRY shaderModuleCreate(const VkDevice device, const VkShaderModuleCreateFlags flags, const size_t codeSize, const uint32_t* code)
{
    if (!device)
    {
        return IShaderModuleSP();
    }

    VkResult result;

    VkShaderModuleCreateInfo shaderModuleCreateInfo;

    memset(&shaderModuleCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));

    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    shaderModuleCreateInfo.flags = flags;
    shaderModuleCreateInfo.codeSize = codeSize;
    shaderModuleCreateInfo.pCode = code;

    VkShaderModule shaderModule;

    result = vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule);

    if (result != VK_SUCCESS)
    {
        logPrint(VKTS_LOG_ERROR, "Shader: Could not create shader module.");

        return IShaderModuleSP();
    }

    auto newInstance = new ShaderModule(device, flags, codeSize, code, shaderModule);

    if (!newInstance)
    {
        vkDestroyShaderModule(device, shaderModule, nullptr);

        return IShaderModuleSP();
    }

    return IShaderModuleSP(newInstance);
}

}
