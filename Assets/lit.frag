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
//uniform vec4  u_BaseColorFactor;
//uniform float u_MetallicFactor;
//uniform float u_RoughnessFactor;
//uniform vec3  u_EmissiveFactor;
//
//
//// Texture samplers
//uniform sampler2D u_BaseColorMap;        // slot 0
//uniform sampler2D u_MetallicRoughnessMap;// slot 1
//uniform sampler2D u_NormalMap;           // slot 2
//uniform sampler2D u_OcclusionMap;         // slot 3
//uniform sampler2D u_EmissiveMap;          // slot 4



void main()
{


    // basic phong lighting model
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // ambient
    vec3 ambient = 0.1 * lightColor;

    // view direction
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect direction
    vec3 reflectDir = reflect(-lightDir, norm);
    // specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);


}
