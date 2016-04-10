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

#include "Scene.hpp"

#include "Object.hpp"

namespace vkts
{

Scene::Scene() :
    IScene(), name(""), allObjects()
{
}

Scene::Scene(const Scene& other) :
    IScene(), name(other.name)
{
    for (size_t i = 0; i < other.allObjects.size(); i++)
    {
        const auto& currentObject = other.allObjects[i];

        if (!currentObject.get())
        {
            name = "";

            allObjects.clear();

            break;
        }

        IObjectSP cloneObject = currentObject->clone();

        if (!cloneObject.get())
        {
            name = "";

            allObjects.clear();

            break;
        }

        allObjects.append(cloneObject);
    }
}

Scene::~Scene()
{
    destroy();
}

//
// IScene
//

const std::string& Scene::getName() const
{
    return name;
}

void Scene::setName(const std::string& name)
{
    this->name = name;
}

void Scene::addObject(const IObjectSP& object)
{
    allObjects.append(object);
}

VkBool32 Scene::removeObject(const IObjectSP& object)
{
    return allObjects.remove(object);
}

IObjectSP Scene::findObject(const std::string& name) const
{
    for (size_t i = 0; i < allObjects.size(); i++)
    {
    	if (allObjects[i]->getName() == name)
    	{
    		return allObjects[i];
    	}
    }

    return IObjectSP();
}

size_t Scene::getNumberObjects() const
{
    return allObjects.size();
}

const SmartPointerVector<IObjectSP>& Scene::getObjects() const
{
    return allObjects;
}

void Scene::updateDescriptorSetsRecursive(const uint32_t allWriteDescriptorSetsCount, VkWriteDescriptorSet* allWriteDescriptorSets)
{
    for (size_t i = 0; i < allObjects.size(); i++)
    {
        allObjects[i]->updateDescriptorSetsRecursive(allWriteDescriptorSetsCount, allWriteDescriptorSets);
    }
}

void Scene::bindDrawIndexedRecursive(const ICommandBuffersSP& cmdBuffer, const SmartPointerVector<IGraphicsPipelineSP>& allGraphicsPipelines, const overwrite* renderOverwrite, const uint32_t bufferIndex, const uint32_t objectOffset, const uint32_t objectStep) const
{
    if (objectStep == 0)
    {
        return;
    }

    const overwrite* currentOverwrite = renderOverwrite;
    while (currentOverwrite)
    {
    	if (!currentOverwrite->sceneBindDrawIndexedRecursive(*this, cmdBuffer, allGraphicsPipelines, bufferIndex, objectOffset, objectStep))
    	{
    		return;
    	}

    	currentOverwrite = currentOverwrite->getNextOverwrite();
    }

    for (size_t i = (size_t) objectOffset; i < allObjects.size(); i += (size_t) objectStep)
    {
        allObjects[i]->bindDrawIndexedRecursive(cmdBuffer, allGraphicsPipelines, renderOverwrite, bufferIndex);
    }
}

void Scene::updateRecursive(const IUpdateThreadContext& updateContext, const uint32_t objectOffset, const uint32_t objectStep)
{
    if (objectStep == 0)
    {
        return;
    }

    for (size_t i = (size_t) objectOffset; i < allObjects.size(); i += (size_t) objectStep)
    {
        allObjects[i]->updateRecursive(updateContext);
    }
}

//
// ICloneable
//

ISceneSP Scene::clone() const
{
    return ISceneSP(new Scene(*this));
}

//
// IDestroyable
//

void Scene::destroy()
{
    for (size_t i = 0; i < allObjects.size(); i++)
    {
        allObjects[i]->destroy();
    }
    allObjects.clear();
}

} /* namespace vkts */
