#version 450 core

layout (location = 0) in vec4 a_vertex;
layout (location = 1) in vec2 a_texCoord;

layout (location = 0) out vec2 v_texCoord;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main(void)
{
	v_texCoord = a_texCoord;

	gl_Position = a_vertex;
}
