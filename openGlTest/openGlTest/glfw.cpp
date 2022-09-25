//
//  glfw.cpp
//  openGlTest
//
//  Created by Stella on 9/25/22.
//


#include <stdio.h>
#include <iostream>

// #ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
// #define GLFW_DLL

#define GL_SILENCE_DEPRECATION

#define GLFW_INCLUDE_NONE

#include "glfw3.h"

// #include "GLFW/glfw3.h"
// #endif


int main(int argc, char **argv)
{
    if (!glfwInit()) return -1;

    std::cout << "GLFW lib has been initialized..." << std::endl;

    return 0;
}





