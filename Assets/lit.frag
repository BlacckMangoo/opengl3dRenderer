#version 430 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    mat3 TBN;
} fs_in;

// Material textures
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_roughness1;
uniform sampler2D texture_ao1;

// Light properties
uniform vec3 lightColor;

void main()
{
    // Sample all textures
    vec3 albedo = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 specularMap = texture(texture_specular1, fs_in.TexCoords).rgb;
    vec3 normalMap = texture(texture_normal1, fs_in.TexCoords).rgb;
    float roughness = texture(texture_roughness1, fs_in.TexCoords).r;
    float ao = texture(texture_ao1, fs_in.TexCoords).r;

    // Transform normal from [0,1] to [-1,1] range
    vec3 normal = normalize(normalMap * 2.0 - 1.0);

    // Calculate lighting vectors in tangent space
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // Ambient lighting with AO
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * albedo * ao;

    // Diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * albedo;

    // Specular lighting (modified by roughness)
    float shininess = mix(128.0, 8.0, roughness); // Convert roughness to shininess
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = spec * lightColor * specularMap;

    // Combine all components
    vec3 result = ambient + diffuse + specular;

    // Apply tone mapping for better color
    result = result / (result + vec3(1.0));

    // Gamma correction
    result = pow(result, vec3(1.0/2.2));

    FragColor = vec4(result, 1.0);
}

