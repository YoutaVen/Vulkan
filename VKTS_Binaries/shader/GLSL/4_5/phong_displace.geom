#version 450 core

layout(triangles) in;

layout(triangle_strip, max_vertices = 3) out;

layout (binding = 0, std140) uniform _u_bufferViewProjection {
        mat4 projectionMatrix;
        mat4 viewMatrix;
} u_bufferViewProjection;

layout (binding = 1, std140) uniform _u_bufferTransform {
        mat4 modelMatrix;
        mat3 normalMatrix;
} u_bufferTransform;

layout (binding = 5) uniform sampler2D u_displacementTexture;

layout(push_constant) uniform _u_displacement {
        float midLevel;
        float strength;
} u_displacement;

layout (location = 0) in vec3 v_g_eye[];
layout (location = 1) in vec3 v_g_normal[];
layout (location = 2) in vec3 v_g_bitangent[];
layout (location = 3) in vec3 v_g_tangent[];
layout (location = 4) in vec2 v_g_texCoord[];

layout (location = 0) out vec3 v_f_eye;
layout (location = 1) out vec3 v_f_normal;
layout (location = 2) out vec3 v_f_bitangent;
layout (location = 3) out vec3 v_f_tangent;
layout (location = 4) out vec2 v_f_texCoord;

in gl_PerVertex
{
  vec4 gl_Position;
} gl_in[];

out gl_PerVertex
{
  vec4 gl_Position;
};

void main(void)
{
    for(int i = 0; i < gl_in.length(); ++i)
    {
        float displace = textureLod(u_displacementTexture, v_g_texCoord[i], 0.0).r * u_displacement.strength - u_displacement.midLevel * u_displacement.strength;
        
        vec4 displaceVector = vec4(v_g_normal[i] * displace, 0.0);
    
        //
    
        vec4 vertex = u_bufferViewProjection.viewMatrix * u_bufferTransform.modelMatrix * (gl_in[i].gl_Position + displaceVector);
    
        v_f_eye = -vec3(vertex);
        v_f_normal = mat3(u_bufferViewProjection.viewMatrix) * u_bufferTransform.normalMatrix * v_g_normal[i];
        v_f_bitangent = mat3(u_bufferViewProjection.viewMatrix) * u_bufferTransform.normalMatrix * v_g_bitangent[i];
        v_f_tangent = mat3(u_bufferViewProjection.viewMatrix) * u_bufferTransform.normalMatrix * v_g_tangent[i];
        v_f_texCoord = v_g_texCoord[i];
    
        gl_Position = u_bufferViewProjection.projectionMatrix * vertex;

        EmitVertex();
    }
    
    EndPrimitive();
}
