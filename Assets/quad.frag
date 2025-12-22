#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{

    // gamma correct
    vec3 col = texture(screenTexture, TexCoord).rgb;
    col = pow(col, vec3(1.0/2.2));

    // ACES tone mapping
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    col = (col * (a * col + b)) / (col * (c * col + d) + e);

    // gamma correct back
    col = pow(col, vec3(2.2));

    FragColor = vec4(col, 1.0);



}
