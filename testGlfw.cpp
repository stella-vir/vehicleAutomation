//
//  testGlfw.cpp
//  vehicleAutomation
//
//  Created by Stella on 9/23/22.
//


// libglfw.3.dylib
// g++ testGlfw.cpp -o t -lglfw -framework Cocoa -framework OpenGL -framework IOKit

// libglfw3.a g++ testGlfw.cpp -o t -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit

#include <stdio.h>
#include <iostream>

// #ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
#define GL_SILENCE_DEPRECATION

#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"
// #endif


int main(int argc, char **argv)
{
    if (!glfwInit()) return -1;

    std::cout << "GLFW lib has been initialized..." << std::endl;

    return 0;
}
