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

#include "PhongMaterial.hpp"

namespace vkts
{

IDescriptorSetsSP PhongMaterial::createDescriptorSetsByName(const std::string& nodeName)
{
	if (allDescriptorSets.contains(nodeName))
	{
		return allDescriptorSets[nodeName];
	}

	//

	if (this->nodeName == "")
	{
		this->nodeName = nodeName;

		allDescriptorPools[nodeName] = descriptorPool;

		allDescriptorSets[nodeName] = descriptorSets;

		return descriptorSets;
	}

	//

	auto currentDescriptorPool = descriptorPoolCreate(descriptorPool->getDevice(), descriptorPool->getFlags(), descriptorPool->getMaxSets(), descriptorPool->getPoolSizeCount(), descriptorPool->getPoolSizes());

    if (!currentDescriptorPool.get())
    {
        return IDescriptorSetsSP();
    }

	auto currentDescriptorSets = descriptorSetsCreate(currentDescriptorPool->getDevice(), currentDescriptorPool->getDescriptorPool(), descriptorSets->getDescriptorSetCount(), descriptorSets->getSetLayouts());

    if (!currentDescriptorSets.get())
    {
        return IDescriptorSetsSP();
    }

    allDescriptorPools[nodeName] = currentDescriptorPool;

    allDescriptorSets[nodeName] = currentDescriptorSets;

    //

    return allDescriptorSets[nodeName];
}

IDescriptorSetsSP PhongMaterial::getDescriptorSetsByName(const std::string& nodeName) const
{
	if (allDescriptorSets.contains(nodeName))
	{
		return allDescriptorSets[nodeName];
	}

	return IDescriptorSetsSP();
}

void PhongMaterial::updateDescriptorImageInfo(const uint32_t colorIndex, const VkSampler sampler, const VkImageView imageView, const VkImageLayout imageLayout)
{
    if (colorIndex >= VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_COLOR_COUNT)
    {
        return;
    }

    memset(&descriptorImageInfos[colorIndex], 0, sizeof(VkDescriptorImageInfo));

    descriptorImageInfos[colorIndex].sampler = sampler;
    descriptorImageInfos[colorIndex].imageView = imageView;
    descriptorImageInfos[colorIndex].imageLayout = imageLayout;

    memset(&writeDescriptorSets[colorIndex], 0, sizeof(VkWriteDescriptorSet));

    writeDescriptorSets[colorIndex].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSets[colorIndex].dstSet = VK_NULL_HANDLE; // Defined later.
    writeDescriptorSets[colorIndex].dstBinding = VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE + colorIndex;
    writeDescriptorSets[colorIndex].dstArrayElement = 0;
    writeDescriptorSets[colorIndex].descriptorCount = 1;
    writeDescriptorSets[colorIndex].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeDescriptorSets[colorIndex].pImageInfo = &descriptorImageInfos[colorIndex];
    writeDescriptorSets[colorIndex].pBufferInfo = nullptr;
    writeDescriptorSets[colorIndex].pTexelBufferView = nullptr;
}

PhongMaterial::PhongMaterial() :
    IPhongMaterial(), name(), alpha(), displacement(), normal(), ambient(), emissive(), diffuse(), specular(), specularShininess(), transparent(VK_FALSE), descriptorPool(), descriptorSets(), nodeName(), allDescriptorPools(), allDescriptorSets()
{
}

PhongMaterial::PhongMaterial(const PhongMaterial& other) :
    IPhongMaterial(), name(other.name), alpha(other.alpha), displacement(other.displacement), normal(other.normal), ambient(other.ambient), emissive(other.emissive), diffuse(other.diffuse), specular(other.specular), specularShininess(other.specularShininess), transparent(other.transparent), descriptorPool(), descriptorSets(), nodeName(), allDescriptorPools(), allDescriptorSets()
{
    // Textures cannot be cloned, just replaced.

    descriptorPool = descriptorPoolCreate(other.descriptorPool->getDevice(), other.descriptorPool->getFlags(), other.descriptorPool->getMaxSets(), other.descriptorPool->getPoolSizeCount(), other.descriptorPool->getPoolSizes());

    if (!descriptorPool.get())
    {
        destroy();

        return;
    }

    descriptorSets = descriptorSetsCreate(descriptorPool->getDevice(), descriptorPool->getDescriptorPool(), other.descriptorSets->getDescriptorSetCount(), other.descriptorSets->getSetLayouts());

    if (!descriptorSets.get())
    {
    	destroy();

        return;
    }

    //

    if (other.allDescriptorPools.size() != other.allDescriptorSets.size())
    {
    	destroy();

        return;
    }


    for (size_t i = 0; i < other.allDescriptorPools.size(); i++)
    {
    	auto currentDescriptorPool = descriptorPoolCreate(other.allDescriptorPools.valueAt(i)->getDevice(), other.allDescriptorPools.valueAt(i)->getFlags(), other.allDescriptorPools.valueAt(i)->getMaxSets(), other.allDescriptorPools.valueAt(i)->getPoolSizeCount(), other.allDescriptorPools.valueAt(i)->getPoolSizes());

        if (!currentDescriptorPool.get())
        {
        	destroy();

            return;
        }

        allDescriptorPools[other.allDescriptorPools.keyAt(i)] = currentDescriptorPool;

        //

        auto currentDescriptorSets = descriptorSetsCreate(currentDescriptorPool->getDevice(), currentDescriptorPool->getDescriptorPool(), other.allDescriptorSets.valueAt(i)->getDescriptorSetCount(), other.allDescriptorSets.valueAt(i)->getSetLayouts());

        if (!currentDescriptorSets.get())
        {
        	destroy();

            return;
        }

        allDescriptorSets[other.allDescriptorSets.keyAt(i)] = currentDescriptorSets;
    }

    //

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->emissive->getSampler()->getSampler(), this->emissive->getImageView()->getImageView(), this->emissive->getMemoryImage()->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_ALPHA - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->alpha->getSampler()->getSampler(), this->alpha->getImageView()->getImageView(), this->alpha->getMemoryImage()->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_DISPLACEMENT - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->displacement->getSampler()->getSampler(), this->displacement->getImageView()->getImageView(), this->displacement->getMemoryImage()->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_NORMAL - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->normal->getSampler()->getSampler(), this->normal->getImageView()->getImageView(), this->normal->getMemoryImage()->getImage()->getImageLayout());

    //

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_AMBIENT - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->ambient->getSampler()->getSampler(), this->ambient->getImageView()->getImageView(), this->ambient->getMemoryImage()->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_DIFFUSE - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->diffuse->getSampler()->getSampler(), this->diffuse->getImageView()->getImageView(), this->diffuse->getMemoryImage()->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_SPECULAR - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->specular->getSampler()->getSampler(), this->specular->getImageView()->getImageView(), this->specular->getMemoryImage()->getImage()->getImageLayout());

    updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_SPECULAR_SHININESS - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->specularShininess->getSampler()->getSampler(), this->specularShininess->getImageView()->getImageView(), this->specularShininess->getMemoryImage()->getImage()->getImageLayout());
}

PhongMaterial::~PhongMaterial()
{
    destroy();
}

//
// IPhongMaterial
//

const std::string& PhongMaterial::getName() const
{
    return name;
}

void PhongMaterial::setName(const std::string& name)
{
    this->name = name;
}

//

const ITextureSP& PhongMaterial::getAlpha() const
{
    return alpha;
}

void PhongMaterial::setAlpha(const ITextureSP& alpha)
{
    this->alpha = alpha;

    if (this->alpha.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_ALPHA - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->alpha->getSampler()->getSampler(), this->alpha->getImageView()->getImageView(), this->alpha->getMemoryImage()->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getDisplacement() const
{
    return displacement;
}

void PhongMaterial::setDisplacement(const ITextureSP& displacement)
{
    this->displacement = displacement;

    if (this->displacement.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_DISPLACEMENT - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->displacement->getSampler()->getSampler(), this->displacement->getImageView()->getImageView(), this->displacement->getMemoryImage()->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getNormal() const
{
    return normal;
}

void PhongMaterial::setNormal(const ITextureSP& normal)
{
    this->normal = normal;

    if (this->normal.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_NORMAL - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->normal->getSampler()->getSampler(), this->normal->getImageView()->getImageView(), this->normal->getMemoryImage()->getImage()->getImageLayout());
    }
}

//

const ITextureSP& PhongMaterial::getEmissive() const
{
    return emissive;
}

void PhongMaterial::setEmissive(const ITextureSP& emissive)
{
    this->emissive = emissive;

    if (this->emissive.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->emissive->getSampler()->getSampler(), this->emissive->getImageView()->getImageView(), this->emissive->getMemoryImage()->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getAmbient() const
{
    return ambient;
}

void PhongMaterial::setAmbient(const ITextureSP& ambient)
{
    this->ambient = ambient;

    if (this->ambient.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_AMBIENT - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->ambient->getSampler()->getSampler(), this->ambient->getImageView()->getImageView(), this->ambient->getMemoryImage()->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getDiffuse() const
{
    return diffuse;
}

void PhongMaterial::setDiffuse(const ITextureSP& diffuse)
{
    this->diffuse = diffuse;

    if (this->diffuse.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_DIFFUSE - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->diffuse->getSampler()->getSampler(), this->diffuse->getImageView()->getImageView(), this->diffuse->getMemoryImage()->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getSpecular() const
{
    return specular;
}

void PhongMaterial::setSpecular(const ITextureSP& specular)
{
    this->specular = specular;

    if (this->specular.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_SPECULAR - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->specular->getSampler()->getSampler(), this->specular->getImageView()->getImageView(), this->specular->getMemoryImage()->getImage()->getImageLayout());
    }
}

const ITextureSP& PhongMaterial::getSpecularShininess() const
{
    return specularShininess;
}

void PhongMaterial::setSpecularShininess(const ITextureSP& specularShininess)
{
    this->specularShininess = specularShininess;

    if (this->specularShininess.get())
    {
        updateDescriptorImageInfo(VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_PHONG_SPECULAR_SHININESS - VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_EMISSIVE, this->specularShininess->getSampler()->getSampler(), this->specularShininess->getImageView()->getImageView(), this->specularShininess->getMemoryImage()->getImage()->getImageLayout());
    }
}

//

VkBool32 PhongMaterial::isTransparent() const
{
	return transparent;
}

void PhongMaterial::setTransparent(const VkBool32 transparent)
{
	this->transparent = transparent;
}

//

const IDescriptorPoolSP& PhongMaterial::getDescriptorPool() const
{
    return descriptorPool;
}

void PhongMaterial::setDescriptorPool(const IDescriptorPoolSP& descriptorPool)
{
    this->descriptorPool = descriptorPool;
}

IDescriptorSetsSP PhongMaterial::getDescriptorSets() const
{
    return descriptorSets;
}

void PhongMaterial::setDescriptorSets(const IDescriptorSetsSP& descriptorSets)
{
    this->descriptorSets = descriptorSets;
}

void PhongMaterial::updateDescriptorSetsRecursive(const std::string& nodeName, const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets)
{
    for (uint32_t i = 0; i < VKTS_BINDING_UNIFORM_SAMPLER_FRAGMENT_COLOR_COUNT; i++)
    {
    	allWriteDescriptorSets[writeDescriptorSets[i].dstBinding] = writeDescriptorSets[i];
    }

    //

    auto currentDescriptorSets = createDescriptorSetsByName(nodeName);

    if (!currentDescriptorSets.get())
    {
        return;
    }

    //

    for (uint32_t i = 0; i < VKTS_BINDING_UNIFORM_BINDING_COUNT; i++)
    {
    	if (i >= allWriteDescriptorSetsCount)
    	{
    		break;
    	}

    	allWriteDescriptorSets[i].dstSet = currentDescriptorSets->getDescriptorSets()[0];
    }

    currentDescriptorSets->updateDescriptorSets(allWriteDescriptorSetsCount, allWriteDescriptorSets, 0, nullptr);
}

void PhongMaterial::bindDescriptorSets(const std::string& nodeName, const ICommandBuffersSP& cmdBuffer, const VkPipelineLayout layout, const uint32_t bufferIndex) const
{
    if (!cmdBuffer.get())
    {
        return;
    }

    auto currentDescriptorSets = getDescriptorSetsByName(nodeName);

    if (!currentDescriptorSets.get())
    {
        return;
    }

    vkCmdBindDescriptorSets(cmdBuffer->getCommandBuffer(bufferIndex), VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &currentDescriptorSets->getDescriptorSets()[0], 0, nullptr);
}

void PhongMaterial::bindDrawIndexedRecursive(const std::string& nodeName, const ICommandBuffersSP& cmdBuffer, const IGraphicsPipelineSP& graphicsPipeline, const overwrite* renderOverwrite, const uint32_t bufferIndex) const
{
    const overwrite* currentOverwrite = renderOverwrite;
    while (currentOverwrite)
    {
    	if (!currentOverwrite->materialBindDrawIndexedRecursive(*this, cmdBuffer, graphicsPipeline, bufferIndex))
    	{
    		return;
    	}

    	currentOverwrite = currentOverwrite->getNextOverwrite();
    }

    bindDescriptorSets(nodeName, cmdBuffer, graphicsPipeline->getLayout(), bufferIndex);
}

//
// ICloneable
//

IPhongMaterialSP PhongMaterial::clone() const
{
    return IPhongMaterialSP(new PhongMaterial(*this));
}

//
// IDestroyable
//

void PhongMaterial::destroy()
{
    if (emissive.get())
    {
        emissive->destroy();
    }

    if (alpha.get())
    {
        alpha->destroy();
    }

    if (displacement.get())
    {
        displacement->destroy();
    }

    if (normal.get())
    {
        normal->destroy();
    }

    transparent = VK_FALSE;

    if (ambient.get())
    {
    	ambient->destroy();
    }

    if (diffuse.get())
    {
    	diffuse->destroy();
    }

    if (specular.get())
    {
    	specular->destroy();
    }

    if (specularShininess.get())
    {
    	specularShininess->destroy();
    }

    //

    descriptorSets = IDescriptorSetsSP();

    descriptorPool = IDescriptorPoolSP();

    memset(&descriptorImageInfos, 0, sizeof(descriptorImageInfos));
    memset(&writeDescriptorSets, 0, sizeof(writeDescriptorSets));

    for (size_t i = 0; i < allDescriptorSets.size(); i++)
    {
        allDescriptorSets.valueAt(i)->destroy();
    }
    allDescriptorSets.clear();

    for (size_t i = 0; i < allDescriptorPools.size(); i++)
    {
    	allDescriptorPools.valueAt(i)->destroy();
    }
    allDescriptorPools.clear();
}

} /* namespace vkts */
