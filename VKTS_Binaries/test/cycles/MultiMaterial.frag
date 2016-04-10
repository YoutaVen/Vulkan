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
    
    
    // Multiply start

    // In
    float Fac_2 = 0.700;
    vec4 Color1_5 = vec4(1.000, 0.000, 0.000, 1.000);
    vec4 Color2_6 = vec4(0.000, 1.000, 0.000, 1.000);
    
    // Out
    vec4 Mix_002_Color = Color1_5 * (1.0 - Fac_2) + Color1_5 * Color2_6 * Fac_2;
    
    // Multiply end
    
    // Mix start

    // In
    float Fac_1 = 0.500;
    vec4 Color1_3 = vec4(1.000, 0.000, 0.000, 1.000);
    vec4 Color2_4 = Mix_002_Color;
    
    // Out
    vec4 Mix_001_Color = mix(Color1_3, Color2_4, Fac_1);
    
    // Mix end
    
    // Add start

    // In
    float Fac_0 = 1.000;
    vec4 Color1_1 = Mix_001_Color;
    vec4 Color2_2 = vec4(0.000, 0.000, 1.000, 1.000);
    
    // Out
    vec4 Mix_Color = Color1_1 + Color2_2 * Fac_0;
    
    // Add end
    
    // Diffuse BSDF start

    // In
    vec4 Color_0 = Mix_Color;
    float Roughness_0 = 0.000;
    vec3 Normal_0 = normal;
    
    // Out
    ob_diffuseNormalRoughness = vec4(Normal_0, Roughness_0);
    ob_diffuseColor = Color_0;
    
    // Diffuse BSDF end
    
    // Generated code end
}
