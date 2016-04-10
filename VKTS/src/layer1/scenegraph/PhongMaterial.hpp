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

#ifndef VKTS_PHONGMATERIAL_HPP_
#define VKTS_PHONGMATERIAL_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class PhongMaterial: public IPhongMaterial
{

protected:

    std::string name;

    ITextureSP alpha;

    ITextureSP displacement;

    ITextureSP normal;

    ITextureSP ambient;

    ITextureSP emissive;

    ITextureSP diffuse;

    ITextureSP specular;

    ITextureSP specularShininess;

    VkBool32 transparent;

    IDescriptorPoolSP descriptorPool;
    IDescriptorSetsSP descriptorSets;
    VkDescriptorImageInfo descriptorImageInfos[VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_COLOR_COUNT];
    VkWriteDescriptorSet writeDescriptorSets[VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_COLOR_COUNT];
    std::string nodeName;

    SmartPointerMap<std::string, IDescriptorPoolSP> allDescriptorPools;
    SmartPointerMap<std::string, IDescriptorSetsSP> allDescriptorSets;

    IDescriptorSetsSP createDescriptorSetsByName(const std::string& nodeName);
    IDescriptorSetsSP getDescriptorSetsByName(const std::string& nodeName) const;

    void updateDescriptorImageInfo(const uint32_t colorIndex, const VkSampler sampler, const VkImageView imageView, const VkImageLayout imageLayout);

public:

    PhongMaterial();
    PhongMaterial(const PhongMaterial& other);
    PhongMaterial(PhongMaterial&& other) = delete;
    virtual ~PhongMaterial();

    PhongMaterial& operator =(const PhongMaterial& other) = delete;

    PhongMaterial& operator =(PhongMaterial && other) = delete;


    //
    // IPhongMaterial
    //

    virtual const std::string& getName() const override;

    virtual void setName(const std::string& name) override;

    //

    virtual const ITextureSP& getAlpha() const override;

    virtual void setAlpha(const ITextureSP& alpha) override;

    virtual const ITextureSP& getDisplacement() const override;

    virtual void setDisplacement(const ITextureSP& displacement) override;

    virtual const ITextureSP& getNormal() const override;

    virtual void setNormal(const ITextureSP& normal) override;

    //

    virtual const ITextureSP& getEmissive() const override;

    virtual void setEmissive(const ITextureSP& emissive) override;

    virtual const ITextureSP& getAmbient() const override;

    virtual void setAmbient(const ITextureSP& ambient) override;

    virtual const ITextureSP& getDiffuse() const override;

    virtual void setDiffuse(const ITextureSP& diffuse) override;

    virtual const ITextureSP& getSpecular() const override;

    virtual void setSpecular(const ITextureSP& specular) override;

    virtual const ITextureSP& getSpecularShininess() const override;

    virtual void setSpecularShininess(const ITextureSP& specularShininess) override;

    //

    virtual VkBool32 isTransparent() const override;

    virtual void setTransparent(const VkBool32 transparent) override;

    //

    virtual const IDescriptorPoolSP& getDescriptorPool() const override;

    virtual void setDescriptorPool(const IDescriptorPoolSP& descriptorPool) override;

    virtual IDescriptorSetsSP getDescriptorSets() const override;

    virtual void setDescriptorSets(const IDescriptorSetsSP& descriptorSets) override;

    virtual void updateDescriptorSetsRecursive(const std::string& nodeName, const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets) override;

    virtual void bindDescriptorSets(const std::string& nodeName, const ICommandBuffersSP& cmdBuffer, const VkPipelineLayout layout, const uint32_t bufferIndex = 0) const override;

    virtual void bindDrawIndexedRecursive(const std::string& nodeName, const ICommandBuffersSP& cmdBuffer, const IGraphicsPipelineSP& graphicsPipeline, const overwrite* renderOverwrite = nullptr, const uint32_t bufferIndex = 0) const override;

    //
    // ICloneable
    //

    virtual IPhongMaterialSP clone() const override;

    //
    // IDestroyable
    //

    virtual void destroy() override;

};

} /* namespace vkts */

#endif /* VKTS_PHONGMATERIAL_HPP_ */
