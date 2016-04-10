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

#ifndef VKTS_IPHONGMATERIAL_HPP_
#define VKTS_IPHONGMATERIAL_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class overwrite;

class IPhongMaterial : public ICloneable<IPhongMaterial>, public IDestroyable
{

public:

    IPhongMaterial() :
        ICloneable<IPhongMaterial>(), IDestroyable()
    {
    }

    virtual ~IPhongMaterial()
    {
    }

    virtual const std::string& getName() const = 0;

    virtual void setName(const std::string& name) = 0;

    //

    virtual const ITextureSP& getAlpha() const = 0;

    virtual void setAlpha(const ITextureSP& alpha) = 0;

    virtual const ITextureSP& getDisplacement() const = 0;

    virtual void setDisplacement(const ITextureSP& displacement) = 0;

    virtual const ITextureSP& getNormal() const = 0;

    virtual void setNormal(const ITextureSP& normal) = 0;

    //

    virtual const ITextureSP& getAmbient() const = 0;

    virtual void setAmbient(const ITextureSP& ambient) = 0;

    virtual const ITextureSP& getEmissive() const = 0;

    virtual void setEmissive(const ITextureSP& emissive) = 0;

    virtual const ITextureSP& getDiffuse() const = 0;

    virtual void setDiffuse(const ITextureSP& diffuse) = 0;

    virtual const ITextureSP& getSpecular() const = 0;

    virtual void setSpecular(const ITextureSP& specular) = 0;

    virtual const ITextureSP& getSpecularShininess() const = 0;

    virtual void setSpecularShininess(const ITextureSP& specularShininess) = 0;

    //

    virtual VkBool32 isTransparent() const = 0;

    virtual void setTransparent(const VkBool32 transparent) = 0;

    //

    virtual const IDescriptorPoolSP& getDescriptorPool() const = 0;

    virtual void setDescriptorPool(const IDescriptorPoolSP& descriptorPool) = 0;

    virtual IDescriptorSetsSP getDescriptorSets() const = 0;

    virtual void setDescriptorSets(const IDescriptorSetsSP& descriptorSets) = 0;

    virtual void updateDescriptorSetsRecursive(const std::string& nodeName, const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets) = 0;

    virtual void bindDescriptorSets(const std::string& nodeName, const ICommandBuffersSP& cmdBuffer, const VkPipelineLayout layout, const uint32_t bufferIndex = 0) const = 0;

    virtual void bindDrawIndexedRecursive(const std::string& nodeName, const ICommandBuffersSP& cmdBuffer, const IGraphicsPipelineSP& graphicsPipeline, const overwrite* renderOverwrite = nullptr, const uint32_t bufferIndex = 0) const = 0;

};

typedef std::shared_ptr<IPhongMaterial> IPhongMaterialSP;

} /* namespace vkts */

#endif /* VKTS_IPHONGMATERIAL_HPP_ */
