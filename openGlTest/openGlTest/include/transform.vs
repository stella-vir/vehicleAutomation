#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

// uniform: global shader var qualifier
// 4 by 4 matrix
uniform mat4 transform;

void main()
{
    // apply transform onto vec pos
   gl_Position = transform * vec4(aPos, 1.0);
   TexCoord = aTexCoord;
}

