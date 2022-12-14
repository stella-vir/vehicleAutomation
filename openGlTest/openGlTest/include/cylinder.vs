#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

// rotate
uniform mat4 model;
// translate
uniform mat4 view;
// perspective cam
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    // gl_Position = vec4(aPos, 1.0);
    // TexCoord = aTexCoord;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}


