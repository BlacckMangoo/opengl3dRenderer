#version 430 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Tangent;

// Lighting uniforms
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

// Material uniforms
uniform vec4  u_BaseColorFactor;
uniform float u_MetallicFactor;
uniform float u_RoughnessFactor;
uniform float u_aoFactor;
uniform vec3  u_EmissiveFactor;





//// Texture samplers
//uniform sampler2D u_BaseColorMap;        // slot 0
//uniform sampler2D u_MetallicRoughnessMap;// slot 1
//uniform sampler2D u_NormalMap;           // slot 2
//uniform sampler2D u_OcclusionMap;         // slot 3
//uniform sampler2D u_EmissiveMap;          // slot 4

// basic PBR without textures


const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a  = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);

    float denom = (NdotH * NdotH) * (a2 - 1.0) + 1.0;
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    return GeometrySchlickGGX(max(dot(N, V), 0.0), roughness) *
    GeometrySchlickGGX(max(dot(N, L), 0.0), roughness);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}




void main()
{

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 L = normalize(lightPos - FragPos);
    vec3 H = normalize(V + L);

    // Calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 of 0.04
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, u_BaseColorFactor.rgb, u_MetallicFactor);

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, u_RoughnessFactor);
    float G   = GeometrySmith(N, V, L, u_RoughnessFactor);
    vec3 F    = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 nominator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // prevent divide by zero
    vec3 specular = nominator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface is metallic)
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by (1 - metallic) metalness reduces diffuse light
    kD *= 1.0 - u_MetallicFactor;

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // add to outgoing radiance Lo
    vec3 ambient = vec3(0.03) * u_BaseColorFactor.rgb * u_aoFactor;
    vec3 color = ambient + (kD * u_BaseColorFactor.rgb / PI + specular) * lightColor * NdotL;

    // emissive
    color += u_EmissiveFactor;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, u_BaseColorFactor.a);


}
