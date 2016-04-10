#version 450 core

layout (binding = 0, std140) uniform _u_bufferViewProjection {
        mat4 projectionMatrix;
        mat4 viewMatrix;
} u_bufferViewProjection;

layout (binding = 1, std140) uniform _u_bufferTransform {
        mat4 modelMatrix;
        mat3 normalMatrix;
} u_bufferTransform;

layout (location = 0) in vec4 a_vertex;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;

layout (location = 0) out vec3 v_eye;
layout (location = 1) out vec3 v_normal;
layout (location = 2) out vec2 v_texCoord;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main(void)
{
	vec4 vertex = u_bufferViewProjection.viewMatrix * u_bufferTransform.modelMatrix * a_vertex;

	v_eye = -vec3(vertex);

	v_normal = mat3(u_bufferViewProjection.viewMatrix) * u_bufferTransform.normalMatrix * a_normal;

	v_texCoord = a_texCoord;

	gl_Position = u_bufferViewProjection.projectionMatrix * vertex;
}
