#version 330 core

in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
    texture(texture1, TexCoord);
}
