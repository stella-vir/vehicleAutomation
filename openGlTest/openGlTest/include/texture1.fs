#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // linear interpolate both 1st contribute 80%, 2nd contrib 20%
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}

