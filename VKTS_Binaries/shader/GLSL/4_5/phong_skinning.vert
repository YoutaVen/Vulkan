#version 450 core

#define VKTS_MAX_JOINTS 96

layout (binding = 0, std140) uniform _u_bufferViewProjection {
        mat4 projectionMatrix;
        mat4 viewMatrix;
} u_bufferViewProjection;

layout (binding = 1, std140) uniform _u_bufferArmatureTransform {
        mat4 modelMatrix;
        mat3 normalMatrix;
} u_bufferArmatureTransform;

layout (binding = 11, std140) uniform _u_bufferBoneTransform {
        mat4 modelMatrix;
        mat3 modelNormalMatrix;
        mat4 jointMatrix[VKTS_MAX_JOINTS];
        mat3 jointNormalMatrix[VKTS_MAX_JOINTS];
} u_bufferBoneTransform;

layout (location = 0) in vec4 a_vertex;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_bitangent;
layout (location = 3) in vec3 a_tangent;
layout (location = 4) in vec2 a_texCoord;
layout (location = 5) in vec4 a_boneIndex_0;
layout (location = 6) in vec4 a_boneIndex_1;
layout (location = 7) in vec4 a_boneWeight_0;
layout (location = 8) in vec4 a_boneWeight_1;
layout (location = 9) in float a_boneCounter;

layout (location = 0) out vec3 v_eye;
layout (location = 1) out vec3 v_normal;
layout (location = 2) out vec3 v_bitangent;
layout (location = 3) out vec3 v_tangent;
layout (location = 4) out vec2 v_texCoord;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main(void)
{
    vec4 vertex = vec4(0.0);
    vec3 normal = vec3(0.0);
    vec3 bitangent = vec3(0.0);
    vec3 tangent = vec3(0.0);
    
    int boneCounter = int(a_boneCounter);
    
    if (boneCounter > 0)
    {
        vec4 transformVertex = u_bufferArmatureTransform.modelMatrix * a_vertex;
        vec3 transformNormal = u_bufferArmatureTransform.normalMatrix * a_normal;
        vec3 transformBitangent = u_bufferArmatureTransform.normalMatrix * a_bitangent;
        vec3 transformTangent = u_bufferArmatureTransform.normalMatrix * a_tangent;
    
	    int boneIndex;
	    float boneWeight;
	    
	    for (int currentBone = 0; currentBone < boneCounter; currentBone++)
	    {
	        boneIndex = -1;
	        boneWeight = 0.0;        
	    
	        if (currentBone < 4)
	        {
	            boneIndex = int(a_boneIndex_0[currentBone]);
	            boneWeight = a_boneWeight_0[currentBone];
	        }
	        else if (currentBone < 8)
	        {
	            boneIndex = int(a_boneIndex_1[currentBone - 4]);
	            boneWeight = a_boneWeight_1[currentBone - 4];
	        }
            
	        if (boneIndex >= 0 && boneWeight != 0.0)
	        {
                vertex += (u_bufferBoneTransform.jointMatrix[boneIndex] * transformVertex) * boneWeight;
                normal += (u_bufferBoneTransform.jointNormalMatrix[boneIndex] * transformNormal) * boneWeight;
                bitangent += (u_bufferBoneTransform.jointNormalMatrix[boneIndex] * transformBitangent) * boneWeight;
                tangent += (u_bufferBoneTransform.jointNormalMatrix[boneIndex] * transformTangent) * boneWeight;
	        }
	    }
	}
	else
	{
        vertex = a_vertex;
        normal = a_normal;
        bitangent = a_bitangent;
        tangent = a_tangent;
	}

	vertex = u_bufferViewProjection.viewMatrix * u_bufferBoneTransform.modelMatrix * vertex;

    v_eye = -vec3(vertex);

	v_normal = mat3(u_bufferViewProjection.viewMatrix) * u_bufferBoneTransform.modelNormalMatrix * normal;
    v_bitangent = mat3(u_bufferViewProjection.viewMatrix) * u_bufferBoneTransform.modelNormalMatrix * bitangent;
    v_tangent = mat3(u_bufferViewProjection.viewMatrix) * u_bufferBoneTransform.modelNormalMatrix * tangent;

	v_texCoord = a_texCoord;

	gl_Position = u_bufferViewProjection.projectionMatrix * vertex;
}
