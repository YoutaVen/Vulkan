#version 450 core

layout(triangles, equal_spacing, ccw) in;

layout (location = 0) in vec3 v_e_eye[];
layout (location = 1) in vec3 v_e_normal[];
layout (location = 2) in vec3 v_e_bitangent[];
layout (location = 3) in vec3 v_e_tangent[];
layout (location = 4) in vec2 v_e_texCoord[];

layout (location = 0) out vec3 v_g_eye;
layout (location = 1) out vec3 v_g_normal;
layout (location = 2) out vec3 v_g_bitangent;
layout (location = 3) out vec3 v_g_tangent;
layout (location = 4) out vec2 v_g_texCoord;

in gl_PerVertex
{
  vec4 gl_Position;
} gl_in[gl_MaxPatchVertices];

out gl_PerVertex {
  vec4 gl_Position;
};

vec2 interpolateVec2(in vec2 v0, in vec2 v1, in vec2 v2)
{
    return v0 * gl_TessCoord.x + v1 * gl_TessCoord.y + v2 * gl_TessCoord.z;
}

vec3 interpolateVec3(in vec3 v0, in vec3 v1, in vec3 v2)
{
    return v0 * gl_TessCoord.x + v1 * gl_TessCoord.y + v2 * gl_TessCoord.z;
}

vec4 interpolateVec4(in vec4 v0, in vec4 v1, in vec4 v2)
{
    return v0 * gl_TessCoord.x + v1 * gl_TessCoord.y + v2 * gl_TessCoord.z;
}

void main()
{
    v_g_eye = interpolateVec3(v_e_eye[0], v_e_eye[1], v_e_eye[2]);
    v_g_normal = interpolateVec3(v_e_normal[0], v_e_normal[1], v_e_normal[2]);
    v_g_bitangent = interpolateVec3(v_e_bitangent[0], v_e_bitangent[1], v_e_bitangent[2]);
    v_g_tangent = interpolateVec3(v_e_tangent[0], v_e_tangent[1], v_e_tangent[2]);
    v_g_texCoord = interpolateVec2(v_e_texCoord[0], v_e_texCoord[1], v_e_texCoord[2]);

	gl_Position = interpolateVec4(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
}
