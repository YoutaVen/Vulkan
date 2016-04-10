#version 450 core

layout (binding = 2, std140) uniform _u_bufferFrag {
	vec3 light;
} u_bufferFrag;

layout (binding = 3) uniform sampler2D u_emissiveTexture;
layout (binding = 4) uniform sampler2D u_alphaTexture;
layout (binding = 5) uniform sampler2D u_displacementTexture;
layout (binding = 6) uniform sampler2D u_normalTexture;
layout (binding = 7) uniform sampler2D u_ambientTexture;
layout (binding = 8) uniform sampler2D u_diffuseTexture;
layout (binding = 9) uniform sampler2D u_specularTexture;
layout (binding = 10) uniform sampler2D u_specularShininessTexture;

layout (location = 0) in vec3 v_normal;
layout (location = 1) in vec2 v_texCoord;

layout (location = 0) out vec4 ob_fragColor;

void main(void)
{
	vec3 normal = normalize(v_normal);
	
	float nDotL = max(dot(u_bufferFrag.light, normal), 0.0);
	
	ob_fragColor = texture(u_diffuseTexture, v_texCoord) * nDotL;
}
