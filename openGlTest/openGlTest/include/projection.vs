#version 330 core

layout (location = 0) in vec3 aPos;
// LAYOUT qualifier
// output attrib index the start pos of the sequential block 2, 3
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

// uniform: global shader var qualifier
// 4 by 4 matrix
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // apply transform onto vec pos
    // model apply to vec4 first, then the view, and lastly projection
    // add lighting: separate to get viewspace pos then add proj to get clipspace for each vertex
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    // TexCoord = aTexCoord;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}


