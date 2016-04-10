#version 450 core

layout (location = 0) in vec4 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_bitangent;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec2 v_texCoord;

// Generated code start

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
    
    
    // Diffuse BSDF start

    // In
    vec4 Color_0 = vec4(0.000, 0.000, 0.800, 1.000);
    float Roughness_0 = 0.100;
    vec3 Normal_0 = normal;
    
    // Out
    ob_diffuseNormalRoughness = vec4(Normal_0, Roughness_0);
    ob_diffuseColor = Color_0;
    
    // Diffuse BSDF end
    
    // Generated code end
}
