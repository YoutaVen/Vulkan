# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

# <pep8-80 compliant>

"""
This script exports a scene into the VulKan ToolS (vkts) format.
"""

import bpy
import os
import math
import mathutils
import bmesh

def friendlyName(name):

    return name.replace(" ", "_")

def friendlyNodeName(name):

    return friendlyName(name).replace(".", "_") 

def friendlyImageName(name):

    return friendlyName(os.path.splitext(bpy.path.basename(name))[0])

def friendlyTransformName(name):

    if len(name) >= len("location") and name[-len("location"):] == "location" :
        return "TRANSLATE"
    if len(name) >= len("rotation_euler") and name[-len("rotation_euler"):] == "rotation_euler" :
        return "ROTATE"
    if len(name) >= len("rotation_quaternion") and name[-len("rotation_quaternion"):] == "rotation_quaternion" :
        return "QUATERNION_ROTATE"
    if len(name) >= len("scale") and name[-len("scale"):] == "scale" :
        return "SCALE"

    return "UNKNOWN"

def friendlyElementName(index, name, isJoint):
    
    newIndex = index

    if len(name) >= len("location") and name[-len("location"):] == "location" :
        if not isJoint:
            if index == 1:
                newIndex = 2
            if index == 2:
                newIndex = 1
    if len(name) >= len("rotation_euler") and name[-len("rotation_euler"):] == "rotation_euler" :
        if not isJoint:
            if index == 1:
                newIndex = 2
            if index == 2:
                newIndex = 1
    if len(name) >= len("rotation_quaternion") and name[-len("rotation_quaternion"):] == "rotation_quaternion" :
        if not isJoint:
            if index == 0:
                newIndex = 3
            if index == 1:
                newIndex = 0
            if index == 2:
                newIndex = 2
            if index == 3:
                newIndex = 1
        else:
            if index == 0:
                newIndex = 3
            if index == 1:
                newIndex = 0
            if index == 2:
                newIndex = 1
            if index == 3:
                newIndex = 2
    if len(name) >= len("scale") and name[-len("scale"):] == "scale" :
        if not isJoint:
            if index == 1:
                newIndex = 2
            if index == 2:
                newIndex = 1

    if newIndex == 0:
        return "X"
    if newIndex == 1:
        return "Y"
    if newIndex == 2:
        return "Z"
    if newIndex == 3:
        return "W"

    return "UNKNOWN"


def extractNode(name):
    if name is None:
        return None

    index = name.find("[\"")
    if (index == -1):
        return None

    subName = name[(index + 2):]

    index = subName.find("\"")
    if (index == -1):
        return None

    return subName[:(index)]


def convertLocation(location):

    location = tuple(location)

    return (location[0], location[2], -location[1])

def convertRotation(rotation):

    rotation = tuple(rotation)

    return (math.degrees(rotation[0]), math.degrees(rotation[2]), -math.degrees(rotation[1]))

def convertScale(scale):

    scale = tuple(scale)

    return (scale[0], scale[2], scale[1])

def convertLocationNoAdjust(location):

    location = tuple(location)

    return (location[0], location[1], location[2])

def convertRotationNoAdjust(rotation):

    rotation = tuple(rotation)

    return (math.degrees(rotation[0]), math.degrees(rotation[1]), math.degrees(rotation[2]))

def convertScaleNoAdjust(scale):

    scale = tuple(scale)

    return (scale[0], scale[1], scale[2])


def saveTextures(context, filepath, imagesLibraryName, materials):

    imagesLibraryFilepath = os.path.dirname(filepath) + "/" + imagesLibraryName

    file_image = open(imagesLibraryFilepath, "w", encoding="utf8", newline="\n")
    fw_image = file_image.write
    fw_image("#\n")
    fw_image("# VulKan ToolS images.\n")
    fw_image("#\n")
    fw_image("\n")

    #

    file = open(filepath, "w", encoding="utf8", newline="\n")
    fw = file.write
    fw("#\n")
    fw("# VulKan ToolS textures.\n")
    fw("#\n")
    fw("\n")
    fw("image_library %s\n" % friendlyName(imagesLibraryName))
    fw("\n")

    textures = {}
    cyclesTextures = {}
    images = {}

    for materialName in materials:

        material = materials[materialName]
        
        for currentTextureSlot in material.texture_slots:
            if currentTextureSlot and currentTextureSlot.texture and currentTextureSlot.texture.type == 'IMAGE':
                image = currentTextureSlot.texture.image
                if image and image.has_data:
                    storeTexture = False
                    if currentTextureSlot.use_map_emit:
                        images.setdefault(friendlyImageName(image.filepath), image)
                        storeTexture = True
                    if currentTextureSlot.use_map_alpha:
                        images.setdefault(friendlyImageName(image.filepath), image)
                        storeTexture = True
                    if currentTextureSlot.use_map_displacement:
                        images.setdefault(friendlyImageName(image.filepath), image)
                        storeTexture = True
                    if currentTextureSlot.use_map_normal:
                        images.setdefault(friendlyImageName(image.filepath), image)
                        storeTexture = True
                    if currentTextureSlot.use_map_ambient:
                        images.setdefault(friendlyImageName(image.filepath), image)
                        storeTexture = True
                    if currentTextureSlot.use_map_color_diffuse:
                        images.setdefault(friendlyImageName(image.filepath), image)
                        storeTexture = True
                    if currentTextureSlot.use_map_color_spec:
                        images.setdefault(friendlyImageName(image.filepath), image)
                        storeTexture = True
                    if currentTextureSlot.use_map_hardness:
                        images.setdefault(friendlyImageName(image.filepath), image)
                        storeTexture = True
                        
                    if storeTexture:
                        textures.setdefault(friendlyImageName(currentTextureSlot.texture.name), currentTextureSlot.texture)
                                           
        # Cycles
        if material.use_nodes == True:
            for currentNode in material.node_tree.nodes:
                if isinstance(currentNode, bpy.types.ShaderNodeTexImage):
                    storeTexture = False
                    if currentNode.image is not None:
                        images.setdefault(friendlyImageName(currentNode.image.filepath), currentNode.image)
                        storeTexture = True

                    if storeTexture:
                        cyclesTextures.setdefault(friendlyImageName(currentNode.name), currentNode)

    for nameOfTexture in textures:

        texture = textures[nameOfTexture]
        
        nameOfImage = friendlyImageName(texture.image.filepath)
        
        useMipMap = "false"
        if texture.use_mipmap:
            useMipMap = "true" 

        fw("#\n")
        fw("# Texture.\n")
        fw("#\n")
        fw("\n")
        fw("name %s\n" % (nameOfTexture + "_texture"))
        fw("\n")
        fw("mipmap %s\n" % (useMipMap))
        fw("image %s\n" % (nameOfImage + "_image"))
        fw("\n")

    for nameOfTexture in cyclesTextures:

        node = cyclesTextures[nameOfTexture]
        
        nameOfImage = friendlyImageName(node.image.filepath)
        
        useMipMap = "true"

        fw("#\n")
        fw("# Texture.\n")
        fw("#\n")
        fw("\n")
        fw("name %s\n" % (nameOfTexture + "_texture"))
        fw("\n")
        fw("mipmap %s\n" % (useMipMap))
        fw("image %s\n" % (nameOfImage + "_image"))
        fw("\n")
    
    for nameOfImage in images:

        image = images[nameOfImage]

        # Always save as TARGA
        context.scene.render.image_settings.file_format = 'TARGA'
        
        image.save_render((os.path.dirname(filepath) + "/" + nameOfImage + ".tga"), context.scene)

        fw_image("#\n")
        fw_image("# Image.\n")
        fw_image("#\n")
        fw_image("\n")
        fw_image("name %s\n" % (nameOfImage + "_image"))
        fw_image("\n")
        fw_image("image_data %s\n" % (nameOfImage + ".tga"))
        fw_image("\n")
    
    file.close()
    
    file_image.close()

    return

#
# Cycles material.
#

fragmentGLSL = """#version 450 core

layout (location = 0) in vec4 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_bitangent;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec2 v_texCoord;

// Generated code start
#nextTexture#
// Generated code end

layout (location = 2) out vec4 ob_position;                // Position as NDC. Last element used but could be freed.
layout (location = 1) out vec4 ob_diffuseNormalRoughness;  // Diffuse normal and roughness.
layout (location = 0) out vec4 ob_diffuseColor;            // Diffuse color and alpha.

void main()
{
    ob_position = v_position;
    
    vec3 normal = normalize(v_normal);
    vec3 bitangent = normalize(v_bitangent);
    vec3 tangent = normalize(v_tangent);
    
    // Generated code start
    #previousMain#
    
    // Generated code end
}"""

texImageFunction = """layout (binding = %d) uniform sampler2D u_texture%d;
#nextTexture#"""

texImageMain = """#previousMain#
    
    // Image texture start

    // In
    vec3 %s = %s;
    
    // Out
    vec4 %s = texture(u_texture%d, %s.st).rgba;
    float %s = texture(u_texture%d, %s.st).a;
    
    // Image texture end"""

multiplyMain = """#previousMain#
    
    // Multiply start

    // In
    float %s = %s;
    vec4 %s = %s;
    vec4 %s = %s;
    
    // Out
    vec4 %s = %s%s * (1.0 - %s) + %s * %s * %s%s;
    
    // Multiply end"""

addMain = """#previousMain#
    
    // Add start

    // In
    float %s = %s;
    vec4 %s = %s;
    vec4 %s = %s;
    
    // Out
    vec4 %s = %s%s + %s * %s%s;
    
    // Add end"""

mixMain = """#previousMain#
    
    // Mix start

    // In
    float %s = %s;
    vec4 %s = %s;
    vec4 %s = %s;
    
    // Out
    vec4 %s = %smix(%s, %s, %s)%s;
    
    // Mix end"""

invertMain = """#previousMain#
    
    // Invert start

    // In
    float %s = %s;
    vec4 %s = %s;
    
    // Out
    vec4 %s = mix(%s, vec4(1.0 - %s.r, 1.0 - %s.g, 1.0 - %s.b, 1.0 - %s.a), %s);
    
    // Invert end"""

diffuseMain = """#previousMain#
    
    // Diffuse BSDF start

    // In
    vec4 %s = %s;
    float %s = %s;
    vec3 %s = %s;
    
    // Out
    ob_diffuseNormalRoughness = vec4(%s, %s);
    ob_diffuseColor = %s;
    
    // Diffuse BSDF end"""


def getFloat(value):
    return "%.3f"%(value)

def getVec2(value):
    return "vec2(%.3f, %.3f)"%(value[0], value[1])

def getVec3(value):
    return "vec3(%.3f, %.3f, %.3f)"%(value[0], value[1], value[2])

def getVec4(value):
    return "vec4(%.3f, %.3f, %.3f, %.3f)"%(value[0], value[1], value[2], value[3])


def replaceParameters(currentNode, openNodes, processedNodes, currentMain):
    for currentSocket in currentNode.inputs:
        currentParameter = currentSocket.name + "_Dummy"
            
        if len(currentSocket.links) == 0:
            currentValue = ""

            # Convert value into GLSL expression.
            if isinstance(currentSocket, bpy.types.NodeSocketColor):
                currentValue = getVec4(currentSocket.default_value)
            elif isinstance(currentSocket, bpy.types.NodeSocketFloatFactor):
                currentValue = getFloat(currentSocket.default_value)
            elif isinstance(currentSocket, bpy.types.NodeSocketVector):
                currentValue = getVec3(currentSocket.default_value)
            
            # Replace parameter with value.
            currentMain = currentMain.replace(currentParameter, currentValue)
        else:
            # Append node for later processing.
            if currentSocket.links[0].from_node not in openNodes and currentSocket.links[0].from_node not in processedNodes:
                openNodes.append(currentSocket.links[0].from_node)
            # Replace parameter with variable.
            currentMain = currentMain.replace(currentParameter, friendlyNodeName(currentSocket.links[0].from_node.name) + "_" + friendlyNodeName(currentSocket.links[0].from_socket.name))
        
    return currentMain


def saveMaterials(context, filepath, texturesLibraryName, imagesLibraryName):
    
    file = open(filepath, "w", encoding="utf8", newline="\n")
    fw = file.write
    fw("#\n")
    fw("# VulKan ToolS materials.\n")
    fw("#\n")
    fw("\n")

    if texturesLibraryName is not None:
        fw("texture_library %s\n" % friendlyName(texturesLibraryName))
        fw("\n")

    meshes = {}

    # Gather all meshes.
    for currentObject in context.scene.objects:

        if currentObject.type == 'MESH':

            meshes.setdefault(currentObject.data)

    materials = {}

    # Gather all materials.
    for mesh in meshes:

        for face in mesh.polygons:

            # Create default material if none is set.        
            if len(mesh.materials) == 0:
            
                if "DefaultMaterial" not in materials:
                    materials.setdefault("DefaultMaterial", bpy.data.materials.new("DefaultMaterial"))
                    
            else:
            
                if mesh.materials[face.material_index].name not in materials:
            
                    materials.setdefault(mesh.materials[face.material_index].name, mesh.materials[face.material_index])

    # Save textures.

    texturesLibraryFilepath = os.path.dirname(filepath) + "/" + texturesLibraryName
    saveTextures(context, texturesLibraryFilepath, imagesLibraryName, materials)

    # Write materials.
    for materialName in materials:

        fw("#\n")
        fw("# Material.\n")
        fw("#\n")
        fw("\n")
        
        material = materials[materialName]
        
        if material.use_nodes == True:

            #
            # Cycles material.
            #
        
            currentFragmentGLSL = fragmentGLSL

            #            
        
            fw("shading BSDF\n")
            fw("\n")        
            fw("name %s\n" % friendlyName(materialName))
            fw("\n")
            fw("fragment_shader %s\n" % friendlyName(materialName + ".frag.spv"))
            fw("\n")
            
            #
            
            nodes = material.node_tree.nodes
            
            materialOutput = nodes.get("Material Output")
            
            if materialOutput is None:
                continue

            if len(materialOutput.inputs["Surface"].links) == 0:
                continue
            
            currentNode = materialOutput.inputs["Surface"].links[0].from_node

            #
            
            alphaCounter = 0
            colorCounter = 0
            diffuseCounter = 0
            facCounter = 0
            normalCounter = 0
            roughnessCounter = 0
            vectorCounter = 0
            
            openNodes = [currentNode]
            processedNodes = []
            
            nodes = []
            
            while len(openNodes) > 0:
            
                currentNode = openNodes.pop(0)

                if isinstance(currentNode, bpy.types.ShaderNodeTexImage):
                    # Image texture.
                    
                    if currentNode not in nodes:
                        nodes.append(currentNode)

                    textureIndex = nodes.index(currentNode)
                        
                    # Inputs.
                    
                    vectorInputName = "Vector_%d" % (vectorCounter)

                    vectorCounter += 1
                    
                    vectorInputParameterName = "Vector_Dummy"
                    
                    # Outputs
                    
                    colorOutputName = friendlyNodeName(currentNode.name) + "_" + friendlyNodeName(currentNode.outputs["Color"].name) 
                    alphaOutputName = friendlyNodeName(currentNode.name) + "_" + friendlyNodeName(currentNode.outputs["Alpha"].name)
                    
                    #
                    
                    currentMain = texImageMain % (vectorInputName, vectorInputParameterName, colorOutputName, textureIndex, vectorInputName, alphaOutputName, textureIndex, vectorInputName)
                    
                    #
                    
                    currentMain = replaceParameters(currentNode, openNodes, processedNodes, currentMain)
 
                    # Special case, if not linked
                     
                    currentMain = currentMain.replace("vec3 " + vectorInputName + " = vec3(0.000, 0.000);", "vec3 " + vectorInputName + " = v_texCoord;")
                    
                    #
                    
                    currentFragmentGLSL = currentFragmentGLSL.replace("#previousMain#", currentMain)
                
                elif isinstance(currentNode, bpy.types.ShaderNodeMixRGB):
                    # Mix color.
                    
                    # Inputs.
                    
                    facInputName = "Fac_%d" % (facCounter)
                    color1InputName = "Color1_%d" % (colorCounter)
                    colorCounter += 1
                    color2InputName = "Color2_%d" % (colorCounter)
                    
                    facCounter += 1
                    colorCounter += 1

                    facInputParameterName = "Fac_Dummy"
                    color1InputParameterName = "Color1_Dummy"
                    color2InputParameterName = "Color2_Dummy"
                    
                    # Outputs
                    
                    colorOutputName = friendlyNodeName(currentNode.name) + "_" + friendlyNodeName(currentNode.outputs["Color"].name) 
                    
                    #
                                        
                    preClamp = ""
                    postClamp = ""
                    
                    if currentNode.use_clamp:
                        preClamp = "clamp("
                        postClamp = ", vec4(0.0, 0.0, 0.0, 0.0), vec4(1.0, 1.0, 1.0, 1.0))"
                    
                    #
                    
                    if currentNode.blend_type == 'MIX':    
                        currentMain = mixMain % (facInputName, facInputParameterName, color1InputName, color1InputParameterName, color2InputName, color2InputParameterName, colorOutputName, preClamp, color1InputName, color2InputName, facInputName, postClamp)
                    elif currentNode.blend_type == 'ADD':
                        currentMain = addMain % (facInputName, facInputParameterName, color1InputName, color1InputParameterName, color2InputName, color2InputParameterName, colorOutputName, preClamp, color1InputName, color2InputName, facInputName, postClamp)
                    elif currentNode.blend_type == 'MULTIPLY':
                        currentMain = multiplyMain % (facInputName, facInputParameterName, color1InputName, color1InputParameterName, color2InputName, color2InputParameterName, colorOutputName, preClamp, color1InputName, facInputName, color1InputName, color2InputName, facInputName, postClamp)
                    else:
                        currentMain = ""
                    #
                    
                    currentMain = replaceParameters(currentNode, openNodes, processedNodes, currentMain)
                    
                    #
                        
                    currentFragmentGLSL = currentFragmentGLSL.replace("#previousMain#", currentMain)
                    
                elif isinstance(currentNode, bpy.types.ShaderNodeInvert):
                    # Invert color.

                    # Inputs
                    
                    facInputName = "Fac_%d" % (facCounter)
                    colorInputName = "Color_%d" % (colorCounter)

                    facCounter += 1
                    colorCounter += 1

                    facInputParameterName = "Fac_Dummy"
                    colorInputParameterName = "Color_Dummy"
                    
                    # Outputs
                    
                    colorOutputName = friendlyNodeName(currentNode.name) + "_" + friendlyNodeName(currentNode.outputs["Color"].name) 
                    
                    #
                    
                    currentMain = invertMain % (facInputName, facInputParameterName, colorInputName, colorInputParameterName, colorOutputName, colorInputName, colorInputName, colorInputName, colorInputName, colorInputName, facInputName) 
                    
                    #
                    
                    currentMain = replaceParameters(currentNode, openNodes, processedNodes, currentMain)
                    
                    #
                        
                    currentFragmentGLSL = currentFragmentGLSL.replace("#previousMain#", currentMain)

                elif isinstance(currentNode, bpy.types.ShaderNodeBsdfDiffuse) and diffuseCounter == 0:
                    # Diffuse BSDF shader.
                    
                    # Inputs.

                    colorInputName = "Color_%d" % colorCounter
                    roughnessInputName = "Roughness_%d" % roughnessCounter
                    normalInputName = "Normal_%d" % normalCounter

                    colorCounter += 1
                    roughnessCounter += 1
                    normalCounter += 1
                    
                    colorInputParameterName = "Color_Dummy"
                    roughnessInputParameterName = "Roughness_Dummy"
                    normalInputParameterName =  "Normal_Dummy"
                    
                    # Outputs.
                    
                    #
                    
                    currentMain = diffuseMain % (colorInputName, colorInputParameterName, roughnessInputName, roughnessInputParameterName, normalInputName, normalInputParameterName, normalInputName, roughnessInputName, colorInputName)
                    
                    #
                    
                    currentMain = replaceParameters(currentNode, openNodes, processedNodes, currentMain)
                    
                    # Special case, if not linked
                    
                    currentMain = currentMain.replace("vec3 " + normalInputName + " = vec3(0.000, 0.000, 0.000);", "vec3 " + normalInputName + " = normal;")
                    
                    #
                        
                    currentFragmentGLSL = currentFragmentGLSL.replace("#previousMain#", currentMain)
                    
                    diffuseCounter += 1
                    
                if currentNode not in processedNodes:
                    processedNodes.append(currentNode)
            #
            
            for binding in range (0, len(nodes)):
                currentTexImage = texImageFunction % (binding, binding)
                currentFragmentGLSL = currentFragmentGLSL.replace("#nextTexture#", currentTexImage)
                
                fw("add_texture %s\n" % (friendlyImageName(nodes[binding].name) + "_texture" ))    
                fw("\n")

            currentFragmentGLSL = currentFragmentGLSL.replace("#nextTexture#", "")
            
            currentFragmentGLSL = currentFragmentGLSL.replace("#previousMain#", "")
                    
            #

            fragmentShaderFilepath = os.path.dirname(filepath) + "/" + friendlyName(materialName) + ".frag"
        
            file_fragmentShader = open(fragmentShaderFilepath, "w", encoding="utf8", newline="\n")
            
            file_fragmentShader.write("%s\n" % currentFragmentGLSL)
            
            file_fragmentShader.close()
        
        else:
        
            #
            # Blender internal material.
            #
        
            fw("shading Phong\n")
            fw("\n")        
            fw("name %s\n" % friendlyName(materialName))
            fw("\n")

            emissiveWritten = False
            alphaWritten = False
            displacementWritten = False
            normalWritten = False
            
            for currentTextureSlot in material.texture_slots:
                if currentTextureSlot and currentTextureSlot.texture and currentTextureSlot.texture.type == 'IMAGE':
                    image = currentTextureSlot.texture.image
                    if image and image.has_data:
                        if currentTextureSlot.use_map_emit:
                            fw("emissive_texture %s\n" % (friendlyImageName(currentTextureSlot.texture.name) + "_texture"))
                            emissiveWritten = True
                        if currentTextureSlot.use_map_alpha:
                            fw("alpha_texture %s\n" % (friendlyImageName(currentTextureSlot.texture.name) + "_texture"))
                            alphaWritten = True
                        if currentTextureSlot.use_map_displacement:
                            fw("displacement_texture %s\n" % (friendlyImageName(currentTextureSlot.texture.name) + "_texture"))
                            displacementWritten = True
                        if currentTextureSlot.use_map_normal:
                            fw("normal_texture %s\n" % (friendlyImageName(currentTextureSlot.texture.name) + "_texture"))
                            normalWritten = True

            if not emissiveWritten:
                fw("emissive_color %f %f %f\n" % (material.emit * material.diffuse_color[0], material.emit * material.diffuse_color[1], material.emit * material.diffuse_color[2]))
            if not alphaWritten:
                fw("alpha_value %f\n" % material.alpha)
            if not displacementWritten:
                fw("displacement_value 0.0\n")
            if not normalWritten:
                fw("normal_vector 0.0 0.0 1.0\n")

            phongAmbientWritten = False
            phongDiffuseWritten = False
            phongSpecularWritten = False
            phongSpecularShininessWritten = False

            for currentTextureSlot in material.texture_slots:
                if currentTextureSlot and currentTextureSlot.texture and currentTextureSlot.texture.type == 'IMAGE':
                    image = currentTextureSlot.texture.image
                    if image:
                        if currentTextureSlot.use_map_ambient:
                            fw("phong_ambient_texture %s\n" % (friendlyImageName(currentTextureSlot.texture.name) + "_texture"))
                            phongAmbientWritten = True
                        if currentTextureSlot.use_map_color_diffuse:
                            fw("phong_diffuse_texture %s\n" % (friendlyImageName(currentTextureSlot.texture.name) + "_texture"))
                            phongDiffuseWritten = True
                        if currentTextureSlot.use_map_color_spec:
                            fw("phong_specular_texture %s\n" % (friendlyImageName(currentTextureSlot.texture.name) + "_texture"))
                            phongSpecularWritten = True
                        if currentTextureSlot.use_map_hardness:
                            fw("phong_specular_shininess_texture %s\n" % (friendlyImageName(currentTextureSlot.texture.name) + "_texture"))
                            phongSpecularShininessWritten = True

            if not phongAmbientWritten:
                fw("phong_ambient_color %f %f %f\n" % (material.ambient * context.scene.world.ambient_color[0], material.ambient * context.scene.world.ambient_color[1], material.ambient * context.scene.world.ambient_color[2]))
            if not phongDiffuseWritten:
                fw("phong_diffuse_color %f %f %f\n" % (material.diffuse_intensity * material.diffuse_color[0], material.diffuse_intensity * material.diffuse_color[1], material.diffuse_intensity * material.diffuse_color[2]))
            if not phongSpecularWritten:
                fw("phong_specular_color %f %f %f\n" % (material.specular_intensity * material.specular_color[0], material.specular_intensity * material.specular_color[1], material.specular_intensity * material.specular_color[2]))
            if not phongSpecularShininessWritten:
                fw("phong_specular_shininess_value %f\n" % (((float(material.specular_hardness) - 1.0) / 510.0) * 128.0))
                
            if material.use_transparency:
                fw("\n")    
                fw("transparent true\n")

            fw("\n")
    
    file.close()

    return

def saveMeshes(context, filepath, materialsLibraryName, subMeshLibraryName):
    
    subMeshLibraryFilepath = os.path.dirname(filepath) + "/" + subMeshLibraryName

    file_subMesh = open(subMeshLibraryFilepath, "w", encoding="utf8", newline="\n")
    fw_subMesh = file_subMesh.write
    fw_subMesh("#\n")
    fw_subMesh("# VulKan ToolS sub meshes.\n")
    fw_subMesh("#\n")
    fw_subMesh("\n")

    if materialsLibraryName is not None:
        fw_subMesh("material_library %s\n" % friendlyName(materialsLibraryName))
        fw_subMesh("\n")

    file = open(filepath, "w", encoding="utf8", newline="\n")
    fw = file.write
    fw("#\n")
    fw("# VulKan ToolS meshes.\n")
    fw("#\n")
    fw("\n")

    if subMeshLibraryName is not None:
        fw("submesh_library %s\n" % friendlyName(subMeshLibraryName))
        fw("\n")

    meshes = {}

    meshToObject = {}
    meshToVertexGroups = {}
    groupNameToIndex = {}

    # Gather all meshes.
    for currentObject in context.scene.objects:

        if currentObject.type == 'MESH':

            meshes.setdefault(currentObject.data)
            
            meshToVertexGroups[currentObject.data] = currentObject.vertex_groups
            meshToObject[currentObject.data] = currentObject

        if currentObject.type == 'ARMATURE':
            jointIndex = 0
            for currentPoseBone in currentObject.pose.bones:
                groupNameToIndex[currentPoseBone.name] = jointIndex
                jointIndex += 1

    # Save meshes.
    for mesh in meshes:

        currentGroups = meshToVertexGroups[mesh]

        #

        fw("#\n")
        fw("# Mesh.\n")
        fw("#\n")
        fw("\n")

        fw("name %s\n" % friendlyName(mesh.name))
        fw("\n")

        bm = bmesh.new()
        bm.from_mesh(mesh)
        bmesh.ops.triangulate(bm, faces=bm.faces)

        materialIndex = 0

        maxMaterialIndex = 0
        for face in bm.faces:
            if face.material_index > maxMaterialIndex:
                maxMaterialIndex = face.material_index

        while materialIndex <= maxMaterialIndex:

            vertexIndex = 0
            
            doWrite = False

            indices = []
            indexToVertex = {}
            indexToNormal = {}
            hasUVs = bm.loops.layers.uv.active
            indexToUV = {}
            hasBones = bm.verts.layers.deform.active
            indexToBones = {}

            # Search for faces with same material.
            for face in bm.faces:
                
                if face.material_index == materialIndex:

                    doWrite = True
                    currentVertex = 0

                    for vert in face.verts:
                        
                        currentFaceVertex = 0
                        
                        if vertexIndex not in indices:
                                
                            indices.append(vertexIndex)
                            
                            indexToVertex.setdefault(vertexIndex, mathutils.Vector((vert.co.x, vert.co.z, -vert.co.y)))
                            
                            if face.smooth:
                                indexToNormal.setdefault(vertexIndex, mathutils.Vector((vert.normal.x, vert.normal.z, -vert.normal.y)))
                            else:
                                indexToNormal.setdefault(vertexIndex, mathutils.Vector((face.normal.x, face.normal.z, -face.normal.y)))
                                    
                            if hasUVs:
                                for loop in face.loops:
                                    if loop.vert == vert:
                                        indexToUV.setdefault(vertexIndex, mathutils.Vector((loop[hasUVs].uv.x, loop[hasUVs].uv.y)))

                            if hasBones:
                                for searchVertex in bm.verts:
                                    if searchVertex.index == vert.index:
                                        deformVertex = searchVertex[hasBones]
                                        indexToBones.setdefault(vertexIndex, deformVertex)
                                            
                            currentFaceVertex += 1
                                                                
                            currentVertex += 1

                            vertexIndex += 1

            if doWrite:
                subMeshName = mesh.name + "_" + str(materialIndex)

                fw("submesh %s\n" % friendlyName(subMeshName))

                fw_subMesh("#\n")
                fw_subMesh("# Sub mesh.\n")
                fw_subMesh("#\n")
                fw_subMesh("\n")
                
                fw_subMesh("name %s\n" % friendlyName(subMeshName))
                fw_subMesh("\n")

                # Store only the vertices used by this material and faces.
                for vertIndex in indices:
                    vert = indexToVertex[vertIndex]
                    fw_subMesh("vertex %f %f %f 1.0\n" % (vert.x, vert.y, vert.z))
                fw_subMesh("\n")
                for vertIndex in indices:
                    normal = indexToNormal[vertIndex]
                    fw_subMesh("normal %f %f %f\n" % (normal.x, normal.y, normal.z))
                fw_subMesh("\n")
                            
                if hasUVs:

                    indexToBitangent = {}
                    indexToTangent = {}

                    vertexIndex = 0

                    for face in bm.faces:
                        if face.material_index == materialIndex:
                            
                            tempIndices = []
                            P = []
                            UV = []

                            for vert in face.verts:
                                
                                tempIndices.append(vertexIndex)
                                P.append(indexToVertex[vertexIndex])
                                UV.append(indexToUV[vertexIndex])

                                vertexIndex += 1                                    

                            for index in range(0, 3):
                                Q1 = P[(index + 1) % 3] - P[index]
                                Q2 = P[(index + 2) % 3] - P[index]
                                    
                                UV1 = UV[(index + 1) % 3] - UV[index]
                                UV2 = UV[(index + 2) % 3] - UV[index]
                                    
                                s1 = UV1.x
                                t1 = UV1.y
                                s2 = UV2.x
                                t2 = UV2.y

                                divisor = s1*t2-s2*t1
                                if divisor == 0.0:
                                    divisor = 1.0

                                factor = 1.0/divisor

                                bitangent = mathutils.Vector((Q1.x * -s2 + Q2.x * s1, Q1.y * -s2 + Q2.y * s1, Q1.z * -s2 + Q2.z * s1)) * factor                        
                                tangent = mathutils.Vector((Q1.x * t2 + Q2.x * -t1, Q1.y * t2 + Q2.y * -t1, Q1.z * t2 + Q2.z * -t1)) * factor

                                bitangent.normalize()
                                tangent.normalize()

                                indexToBitangent.setdefault(tempIndices[index], bitangent)
                                indexToTangent.setdefault(tempIndices[index], tangent)

                    for vertIndex in indices:
                        bitangent = indexToBitangent[vertIndex]
                        fw_subMesh("bitangent %f %f %f\n" % (bitangent.x, bitangent.y, bitangent.z))
                    fw_subMesh("\n")
                                
                    for vertIndex in indices:
                        tangent = indexToTangent[vertIndex]
                        fw_subMesh("tangent %f %f %f\n" % (tangent.x, tangent.y, tangent.z))
                    fw_subMesh("\n")

                    for vertIndex in indices:
                        uv = indexToUV[vertIndex]
                        fw_subMesh("texcoord %f %f\n" % (uv.x, uv.y))
                    fw_subMesh("\n")
                    
                else:
                    # Save default texture coordinates.
                    for vertIndex in indices:
                        fw_subMesh("texcoord 0.0 0.0\n")
                    fw_subMesh("\n")

                # Save bones when available.
                if hasBones:

                    allBoneIndices = {}

                    allBoneWeights = {}

                    allNumberBones = []
                    
                    for vertIndex in indices:

                        tempBoneIndices = []

                        tempBoneWeights = []
                        
                        deformVertex = indexToBones[vertIndex]

                        numberBones = 0.0
                        
                        for item in deformVertex.items():
                            
                            group = currentGroups[item[0]]
                            
                            tempBoneIndices.append(groupNameToIndex[group.name])
                            
                            tempBoneWeights.append(item[1])
                            numberBones += 1.0

                            if numberBones == 8.0:
                                break

                        for iterate in range(int(numberBones), 8):
                            tempBoneIndices.append(-1.0)
                            tempBoneWeights.append(0.0)

                        allBoneIndices.setdefault(vertIndex, tempBoneIndices)
                        allBoneWeights.setdefault(vertIndex, tempBoneWeights)

                        allNumberBones.append(numberBones)

                    for vertIndex in indices:
                        tempBoneIndices = allBoneIndices[vertIndex]

                        fw_subMesh("boneIndex")
                        for currentIndex in tempBoneIndices:
                            fw_subMesh(" %.1f" % currentIndex)
                        fw_subMesh("\n")
                    fw_subMesh("\n")

                    for vertIndex in indices:
                        tempBoneWeights = allBoneWeights[vertIndex]

                        fw_subMesh("boneWeight")
                        for currentWeight in tempBoneWeights:
                            fw_subMesh(" %f" % currentWeight)
                        fw_subMesh("\n")
                    fw_subMesh("\n")

                    for currentNumberBones in allNumberBones:
                        fw_subMesh("numberBones %.1f\n" % currentNumberBones)
                    fw_subMesh("\n")

                # Save face and adjust face index, if needed.
                for index in indices:
                    # Indices go from 0 to maximum vertices.
                    if index % 3 == 0:
                        fw_subMesh("face")
                    fw_subMesh(" %d" % index)
                    if index % 3 == 2:
                        fw_subMesh("\n")
                    
                fw_subMesh("\n")

                if len(mesh.materials) > 0:
                    fw_subMesh("material %s\n" % friendlyName(mesh.materials[materialIndex].name))
                else:
                    fw_subMesh("material %s\n" % friendlyName("DefaultMaterial"))
                fw_subMesh("\n")
            
            materialIndex += 1

        fw("\n")

        currentObject = meshToObject[mesh]

        #
        # Save displacement, if available.
        #

        if "Displace" in currentObject.modifiers:
            fw("displace %f %f\n" % (currentObject.modifiers["Displace"].mid_level, currentObject.modifiers["Displace"].strength))
            fw("\n")

        #
        # Save AABB.
        #

        center = mathutils.Vector(convertLocation((currentObject.bound_box[0][0], currentObject.bound_box[0][1], currentObject.bound_box[0][2])))

        for corner in range(1, 8):
            center = center + mathutils.Vector(convertLocation((currentObject.bound_box[corner][0], currentObject.bound_box[corner][1], currentObject.bound_box[corner][2])))

        center = center / 8.0

        scale = center - mathutils.Vector(convertLocation((currentObject.bound_box[0][0], currentObject.bound_box[0][1], currentObject.bound_box[0][2])))

        fw("aabb %f %f %f %f %f %f\n" % (center.x, center.y, center.z, math.fabs(scale.x), math.fabs(scale.y), math.fabs(scale.z)))
        fw("\n")

    file.close()

    file_subMesh.close()

    return

def saveAnimation(context, fw, fw_animation, fw_channel, name, currentAnimation, filterName, isJoint):

    hasData = False

    # Check, animation data for a specific filter is available. 

    for currentCurve in currentAnimation.action.fcurves:
        if filterName == extractNode(currentCurve.data_path):
            hasData = True
            break

    if not hasData:
        return

    fw("animation %s\n" % friendlyName("animation_" + name))
    fw("\n")

    fw_animation("#\n")
    fw_animation("# Animation.\n")
    fw_animation("#\n")
    fw_animation("\n")
    fw_animation("name %s\n" % friendlyName("animation_" + name))
    fw_animation("\n")
    fw_animation("start %f\n" % (context.scene.frame_start / context.scene.render.fps))
    fw_animation("stop %f\n" % (context.scene.frame_end / context.scene.render.fps))
    fw_animation("\n")

    # Loop over curves several times to achieve sorting.
    for usedTransform in ["TRANSLATE", "ROTATE", "QUATERNION_ROTATE", "SCALE"]:

        dataWritten = False

        for usedElement in ["X", "Y", "Z", "W"]:

            for currentCurve in currentAnimation.action.fcurves:

                currentFilterName = extractNode(currentCurve.data_path)

                if currentFilterName != filterName:
                    continue


                transform = friendlyTransformName(currentCurve.data_path)
                element = friendlyElementName(currentCurve.array_index, currentCurve.data_path, isJoint)

                if transform != usedTransform or element != usedElement:
                    continue

                if not dataWritten:
                    fw_animation("# %s\n" % (transform.lower().title() + "."))
                    fw_animation("\n")

                dataWritten = True
                
                fw_animation("channel %s\n" % friendlyName("channel_" + transform + "_" + element + "_" + name))

                # Save the channel.

                fw_channel("#\n")
                fw_channel("# Channel.\n")
                fw_channel("#\n")
                fw_channel("\n")
                fw_channel("name %s\n" % friendlyName("channel_" + transform + "_" + element + "_" + name))
                fw_channel("\n")
                fw_channel("target_transform %s\n" % transform)
                fw_channel("target_element %s\n" % element)
                fw_channel("\n")

                for currentKeyframe in currentCurve.keyframe_points:

                    value = currentKeyframe.co[1]
                    leftValue = currentKeyframe.handle_left[1]
                    rightValue = currentKeyframe.handle_right[1]

                    if element == "Z" and transform != "SCALE" and not isJoint:
                        value = -value
                        leftValue = -leftValue
                        rightValue = -rightValue
                        
                    if transform == "ROTATE":
                        value = math.degrees(value)
                        leftValue = math.degrees(leftValue)
                        rightValue = math.degrees(rightValue)

                    if currentKeyframe.interpolation == 'BEZIER':
                        fw_channel("keyframe %f %f BEZIER %f %f %f %f\n" % (currentKeyframe.co[0] / context.scene.render.fps, value, currentKeyframe.handle_left[0] / context.scene.render.fps, leftValue, currentKeyframe.handle_right[0] / context.scene.render.fps, rightValue))        
                    elif currentKeyframe.interpolation == 'LINEAR':
                        fw_channel("keyframe %f %f LINEAR\n" % (currentKeyframe.co[0] / context.scene.render.fps, value))    
                    elif currentKeyframe.interpolation == 'CONSTANT':
                        fw_channel("keyframe %f %f CONSTANT\n" % (currentKeyframe.co[0] / context.scene.render.fps, value))    
                    
                fw_channel("\n")

        if dataWritten:    
            fw_animation("\n")

    return

def saveBone(context, fw, fw_animation, fw_channel, currentPoseBone, armatureName, jointIndex, animation_data):

    parentPoseBone = currentPoseBone.parent
    if parentPoseBone is None:
        parentName = armatureName
    else:
        parentName = parentPoseBone.name


    fw("# Node.\n")
    fw("\n")

    # This matrix, as it is from the pose, already has the wanted coordinate system.
    # Switch to "Local" transformation orientation in Blender to see it.

    location, rotation, scale = currentPoseBone.matrix_basis.decompose()        

    fw("node %s %s\n" % (friendlyName(currentPoseBone.name), friendlyName(parentName)))
    fw("\n")
    fw("translate %f %f %f\n" % (convertLocationNoAdjust(location)))
    fw("rotate %f %f %f\n" % (convertRotationNoAdjust(rotation.to_euler())))
    fw("scale %f %f %f\n" % (convertScaleNoAdjust(scale)))
    fw("\n")
    fw("jointIndex %d\n" % jointIndex)
    fw("\n")

    # This matrix, as it is from the armature, already has the wanted coordinate system.
    # Also, even if noted as local, the matrix is the result of the matrix multiplication of the bone hierarchy.
    # To get the "real" local matrix, the inverted parent matrix has to be multiplied.
    # As the root bone is relative to the Blender original coordinate system, the root bone has to be converted.
    # This has not to be done for the child bones, as this is canceled out through the inverted parent matrix.

    convertMatrix = mathutils.Matrix(((1, 0, 0, 0), (0, 0, 1, 0), (0, -1, 0, 0) , (0, 0, 0, 1)))

    bindMatrix = currentPoseBone.bone.matrix_local

    parentBone = currentPoseBone.bone.parent
    if parentBone:
        bindMatrix = parentBone.matrix_local.inverted() * bindMatrix
    else:
        bindMatrix = convertMatrix * bindMatrix
    
    location, rotation, scale = bindMatrix.decompose()        

    location = convertLocationNoAdjust(location)
    rotation = convertRotationNoAdjust(rotation.to_euler())
    scale = convertScaleNoAdjust(scale)

    fw("bind_translate %f %f %f\n" % (location))
    fw("bind_rotate %f %f %f\n" % (rotation))
    fw("bind_scale %f %f %f\n" % (scale))
    fw("\n")
    
    if animation_data is not None:
        saveAnimation(context, fw, fw_animation, fw_channel, currentPoseBone.name, animation_data, currentPoseBone.name, True)
    
    return

def saveNode(context, fw, fw_animation, fw_channel, currentObject):

    #convertMatrix = mathutils.Matrix(((1, 0, 0, 0), (0, 0, 1, 0), (0, -1, 0, 0) , (0, 0, 0, 1)))

    #location, rotation, scale = (convertMatrix * currentObject.matrix_local).decompose()
    location, rotation, scale = currentObject.matrix_local.decompose()

    location = convertLocation(location)
    rotation = convertRotation(rotation.to_euler())
    scale = convertScale(scale)

    parentObject = currentObject.parent
    if parentObject is None:
        parentName = "-"
    else:
        parentName = parentObject.name

    fw("# Node.\n")
    fw("\n")

    fw("node %s %s\n" % (friendlyName(currentObject.name), friendlyName(parentName)))
    fw("\n")
    fw("translate %f %f %f\n" % location)
    fw("rotate %f %f %f\n" % rotation)
    fw("scale %f %f %f\n" % scale)
    fw("\n")

    if currentObject.type == 'MESH':
        fw("mesh %s\n" % friendlyName(currentObject.data.name))
        fw("\n")

    if currentObject.animation_data is not None:
        saveAnimation(context, fw, fw_animation, fw_channel, currentObject.name, currentObject.animation_data, None, False)

    if currentObject.type == 'ARMATURE':
        fw("joints %d\n" % len(currentObject.pose.bones.values()))
        fw("\n")

        jointIndex = 0
        for currentPoseBone in currentObject.pose.bones:
            saveBone(context, fw, fw_animation, fw_channel, currentPoseBone, currentObject.name, jointIndex, currentObject.animation_data)
            jointIndex += 1

    for childObject in currentObject.children:
        saveNode(context, fw, fw_animation, fw_channel, childObject)
    
    return

def saveObjects(context, filepath, meshLibraryName, animationLibraryName, channelLibraryName):

    channelLibraryFilepath = os.path.dirname(filepath) + "/" + channelLibraryName

    file_channel = open(channelLibraryFilepath, "w", encoding="utf8", newline="\n")
    fw_channel = file_channel.write
    fw_channel("#\n")
    fw_channel("# VulKan ToolS channels.\n")
    fw_channel("#\n")
    fw_channel("\n")


    animationLibraryFilepath = os.path.dirname(filepath) + "/" + animationLibraryName

    file_animation = open(animationLibraryFilepath, "w", encoding="utf8", newline="\n")
    fw_animation = file_animation.write
    fw_animation("#\n")
    fw_animation("# VulKan ToolS animations.\n")
    fw_animation("#\n")
    fw_animation("\n")

    if channelLibraryName is not None:
        fw_animation("channel_library %s\n" % friendlyName(channelLibraryName))
        fw_animation("\n")


    file = open(filepath, "w", encoding="utf8", newline="\n")
    fw = file.write
    fw("#\n")
    fw("# VulKan ToolS objects.\n")
    fw("#\n")
    fw("\n")

    if meshLibraryName is not None:
        fw("mesh_library %s\n" % friendlyName(meshLibraryName))
        fw("\n")

    if animationLibraryName is not None:
        fw("animation_library %s\n" % friendlyName(animationLibraryName))
        fw("\n")

    for currentObject in context.scene.objects:

        if currentObject.type == 'CAMERA':
            continue

        if currentObject.type == 'CURVE':
            continue

        if currentObject.type == 'FONT':
            continue

        if currentObject.type == 'LAMP':
            continue

        if currentObject.type == 'LATTICE':
            continue

        if currentObject.type == 'META':
            continue

        if currentObject.type == 'PATH':
            continue

        if currentObject.type == 'SPEAKER':
            continue

        if currentObject.type == 'SURFACE':
            continue

        if currentObject.parent is not None:
            continue;
        
        fw("#\n")
        fw("# Object.\n")
        fw("#\n")
        fw("\n")

        fw("name %s\n" % friendlyName(currentObject.name))
        fw("\n")
                    
        saveNode(context, fw, fw_animation, fw_channel, currentObject)
    
    file.close()

    file_animation.close()

    file_channel.close()

    return

def save(operator,
         context,
         filepath="",
         global_matrix=None
         ):

    if global_matrix is None:
        from mathutils import Matrix
        global_matrix = Matrix()

    sceneFilepath = filepath
        
    file = open(sceneFilepath, "w", encoding="utf8", newline="\n")
    fw = file.write
    fw("#\n")
    fw("# VulKan ToolS scene.\n")
    fw("#\n")
    fw("\n")

    fw("scene_name %s\n" % friendlyName(context.scene.name))
    fw("\n")
    
    imagesLibraryName = bpy.path.basename(sceneFilepath).replace(".vkts", "_images.vkts")

    texturesLibraryName = bpy.path.basename(sceneFilepath).replace(".vkts", "_textures.vkts")

    #

    materialsLibraryName = bpy.path.basename(sceneFilepath).replace(".vkts", "_materials.vkts")

    materialsLibraryFilepath = os.path.dirname(sceneFilepath) + "/" + materialsLibraryName

    saveMaterials(context, materialsLibraryFilepath, texturesLibraryName, imagesLibraryName)

    #

    subMeshLibraryName = bpy.path.basename(sceneFilepath).replace(".vkts", "_submeshes.vkts")

    meshLibraryName = bpy.path.basename(sceneFilepath).replace(".vkts", "_meshes.vkts")
    
    meshLibraryFilepath = os.path.dirname(sceneFilepath) + "/" + meshLibraryName

    saveMeshes(context, meshLibraryFilepath, materialsLibraryName, subMeshLibraryName)

    #

    animationLibraryName = bpy.path.basename(sceneFilepath).replace(".vkts", "_animations.vkts")

    channelLibraryName = bpy.path.basename(sceneFilepath).replace(".vkts", "_channels.vkts")

    #

    objectLibraryName = bpy.path.basename(sceneFilepath).replace(".vkts", "_objects.vkts")

    fw("object_library %s\n" % friendlyName(objectLibraryName))
    fw("\n")
    
    objectLibraryFilepath = os.path.dirname(sceneFilepath) + "/" + objectLibraryName

    saveObjects(context, objectLibraryFilepath, meshLibraryName, animationLibraryName, channelLibraryName)

    #
    
    for currentObject in context.scene.objects:

        if currentObject.type == 'CAMERA':
            continue

        if currentObject.type == 'CURVE':
            continue

        if currentObject.type == 'FONT':
            continue

        if currentObject.type == 'LAMP':
            continue

        if currentObject.type == 'LATTICE':
            continue

        if currentObject.type == 'META':
            continue

        if currentObject.type == 'PATH':
            continue

        if currentObject.type == 'SPEAKER':
            continue

        if currentObject.type == 'SURFACE':
            continue

        if currentObject.parent is not None:
            continue;
        
        fw("#\n")
        fw("# Object instance.\n")
        fw("#\n")
        fw("\n")

        fw("object %s\n" % friendlyName(currentObject.name))
        fw("\n")
        fw("name %s\n" % friendlyName(currentObject.name))
        fw("\n")
        fw("translate 0.0 0.0 0.0\n")
        fw("rotate 0.0 0.0 0.0\n")
        fw("scale 1.0 1.0 1.0\n")
        fw("\n")
    
    file.close()

    return {'FINISHED'}
