#version 450 core

layout(vertices = 3) out;

layout (location = 0) in vec3 v_c_eye[];
layout (location = 1) in vec3 v_c_normal[];
layout (location = 2) in vec3 v_c_bitangent[];
layout (location = 3) in vec3 v_c_tangent[];
layout (location = 4) in vec2 v_c_texCoord[];

layout (location = 0) out vec3 v_e_eye[];
layout (location = 1) out vec3 v_e_normal[];
layout (location = 2) out vec3 v_e_bitangent[];
layout (location = 3) out vec3 v_e_tangent[];
layout (location = 4) out vec2 v_e_texCoord[];

in gl_PerVertex
{
  vec4 gl_Position;
} gl_in[gl_MaxPatchVertices];

out gl_PerVertex
{
  vec4 gl_Position;
} gl_out[];

void main(void)
{
    v_e_eye[gl_InvocationID] = v_c_eye[gl_InvocationID];
    v_e_normal[gl_InvocationID] = v_c_normal[gl_InvocationID];
    v_e_bitangent[gl_InvocationID] = v_c_bitangent[gl_InvocationID];
    v_e_tangent[gl_InvocationID] = v_c_tangent[gl_InvocationID];
    v_e_texCoord[gl_InvocationID] = v_c_texCoord[gl_InvocationID];

	gl_TessLevelOuter[0] = 4.0;
	gl_TessLevelOuter[1] = 4.0;
	gl_TessLevelOuter[2] = 4.0;

	gl_TessLevelInner[0] = 4.0;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
