//
//  uniform.cpp
//  openGlTest
//
//  Created by Stella on 9/28/22.
//


// clang -c glad.c
// make the lib static, instead of searching for system files
// ar rcs libglad.a glad.o

// g++ glfw.cpp glad.c -o output -Lfolder -lglad -lglfw
// -D MAC
// g++ glfw.cpp -o g glad.o -lglfw -framework Cocoa -framework OpenGL -framework IOkit

// g++ glfw.cpp -o g glad.c -I/glad/ -lglfw -framework Cocoa -framework OpenGL -framework IOkit
// g++ glfw.cpp -o g glad.c -lglfw -ldl

#include <stdio.h>
#include <iostream>
#include <math.h>

// #ifdef __APPLE__
// Defined before OpenGL and GLUT includes to avoid deprecation messages
// #define GLFW_DLL

#define GL_SILENCE_DEPRECATION

#define GLFW_INCLUDE_NONE

//#include <glad/glad.h>
#include "glad/glad.h"

// graphic library framework
// #include <GLFW/glfw3.h>
#include "GLFW/glfw3.h"
// #endif


const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 500;

// shader code
// raw str in cpp
// HERE/THERE: delimiter
// opengl version
// every shader main() execute
// input var from glVertexAttribPointer vertices vec3: 3 float vals
// assign the gl position to vec4 in clip space xyx / w w=1: homogeous 2D be divided by 1 unchanged
// aPos: expands into aPos.x, aPos.y, aPos.z
const char *vertexShaderSource = R"HERE(
    #version 330 core
    
    layout (location = 0) in vec3 aPos;
    
    void main()
    {
       gl_Position = vec4(aPos, 1.0);
    }
)HERE";


// first and only val, thus is the output of the pixel
// uniform global not allowed to change
// frame to frame/shader, change val to have color pulse
const char *fragmentShaderSource = R"HERE(
    #version 330 core
    
    out vec4 FragColor;
    uniform vec4 ourColor;

    void main()
    {
       FragColor = ourColor;
    }
)HERE";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // resize the full width and height of the win
    // bottom left 0 0
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main(int argc, char **argv)
{
    if (!glfwInit()) return -1;

    std::cout << "GLFW lib has been initialized..." << std::endl;
 
    // init
    glfwInit();
    // opengl version hints
    // version 3 for xcode!!!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glad core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
        // uncomment this statement to fix compilation on OS X
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    
    // create win null: monitor
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "vehicleAutomation", NULL, NULL);
    std::cout << "Initializing GLFW window" << std::endl;

    
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // load all opengl func pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // monitor mode & global state
        glfwTerminate();
        return -1;
    }
    
    int shaderProgram = glCreateProgram();

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // handle 1: single string &: char pointer pointer, not array
    // length: NULL terminate
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // more compilation work in diff graphics drivers
    // spir-v: pre-proccessed by the compiler into an intermediate form, more reliant on graphics driver at runtime
    glCompileShader(vertexShader);
    
    int  success;
    char infoLog[512];
    // success set to 0 for failure
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        // log out what went wrong
        // up to 512 bytes long
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
        
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout <<
            "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
            infoLog << std::endl;
    }
    
    // DISPOSE shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // GPU side: data vertices and buffers
    float vertices[] = {
        // left
        // x -0.5f: half way from left edge to the center
        // y -0.5f: half way down from vertical center to bottom
        // lower left quadrant of the clip space
//        -0.5f, -0.5f, 0.0f,
//        // right
//        0.5f, -0.5f, 0.0f,
//        // top
//        0.0f,  0.5f, 0.0f
        0, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
    };
    
    // to draw rectangle out of these two triangles with the indices of the array
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    // handle
    // element buffer object
    unsigned int VAO, VBO, EBO;
    // create objs, only 1 in this case
    // pointer to fill in the val with new handle val
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // after bound, no need to call VAO, VBO again
    // populate the data copy vertices array into VBO
    // sizeof: specify the bytes
    // hint: static memory layout in GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // vertex array: indices ref to buffer
    // set the index and the nature of the data in the buffer
    // set index 0 and associate it to be bound with the current buffer
    // 3: vertices the cts from the elements in the buffer
    // false: not mormalized and supplies fixed vals
    // 3 * sizeof=12: the distance betweem the first byte of the one and the next byte of the other, skip over to the next attr of the same type
    // void* pointer offset/int: reading from the first element, not skipping any start/0/3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // enable
    glEnableVertexAttribArray(0);

    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    // if not win closed re-render one more time
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        // render a blank win, goes first
        // r g b a 1:fully opaque
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // only clear the color buffer, not depth or stencil
        glClear(GL_COLOR_BUFFER_BIT);
        
        // not changing every frame
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        
        float timeVal = glfwGetTime();
        // pulsing gradual transition [-1, 1]
        float greenVal = sin(timeVal);
        // 4f: vector of the 4 floats after
        // function overloading
        // use the same shader program
        glUniform4f(vertexColorLocation, 0.0f, greenVal, 0.0f, 1.0f);
        
        // compiling shader
        glUseProgram(shaderProgram);
        // handle for vertex array object vertex buffer
        glBindVertexArray(VAO);
        // use 3 vertices to draw the triangle, start from index 0
        // glDrawArrays(0) unbind
        
        // GL_ELEMENT_ARRAY_BUFFER
        // (primititives, no.of vertex indices, type of the indices,
        // offset-can set to read from other indices other than the first one)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        

        // keys pressed/released mouse moved etc
        // swap which is being displayed, which is gonna be drawn to
        // async wait 60 hz refresh rate fps
        glfwSwapBuffers(window);
        // read events from ev queue
        glfwPollEvents();
    }

    // de-allocate
    // the gpu seems to be able to handle
    glDeleteVertexArrays(1, &VAO);
    // the handle part, safe to del
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}



