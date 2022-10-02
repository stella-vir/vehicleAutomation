//
//  cylinder.cpp
//  openGlTest
//
//  Created by Stella on 10/2/22.
//

#include <stdio.h>
#include <iostream>
#include <math.h>

#define GL_SILENCE_DEPRECATION

#define GLFW_INCLUDE_NONE

//#include <glad/glad.h>
#include <include/glad/glad.h>

// graphic library framework
// #include <GLFW/glfw3.h>
#include <include/GLFW/glfw3.h>
// #endif

#define STB_IMAGE_IMPLEMENTATION
// dependency
#include <include/stb_image.h>

#include <include/glm/glm.hpp>
#include <include/glm/gtc/matrix_transform.hpp>
#include <include/glm/gtc/type_ptr.hpp>

// abstract
#include <include/filesystem.h>
// encapsulate the shader as a class
#include <include/shader_s.h>

const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 500;

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
    
    Shader ourShader("cylinder.vs", "cylinder.fs");
    
    int sides = 10;
    
    // pos * sides * dir
    float *vertices = new float[8*sides*2];
    unsigned int *indices = new unsigned int[6*sides];
    float angle = (2.0f * M_PI )/ sides;
    float radius = .8;
    
    for (int i=0; i<sides; i++)
    {
        int top = 8*2*i;
        int bot = top+8;
        int tri = 6*i;
        int ver1 = 2*i;
        int ver2 = ver1 + 2;
        
        vertices[top+0] = cos(angle*i)*radius; // cos(2pi/10) * .3 cos(2pi * 2/10) * .3
        vertices[top+1] = .5f; // sin(angle*i)*radius;
        vertices[top+2] = 0;
        vertices[top+3] = 1.0f;
        vertices[top+4] = 1.0f;
        vertices[top+5] = 1.0f;
        vertices[top+6] = 0.0f;
        vertices[top+7] = 0.0f;
        
        vertices[bot+0] = cos(angle*i)*radius;
        vertices[bot+1] = -.5f;
        vertices[bot+2] = 0;
        vertices[bot+3] = 1.0f;
        vertices[bot+4] = 1.0f;
        vertices[bot+5] = 1.0f;
        vertices[bot+6] = 0.0f;
        vertices[bot+7] = 0.0f;
        
        if (i == sides-1)
        {
            ver2 = 0;
        }
        
        indices[tri+0] = ver1 + 0; // 0 1
        indices[tri+1] = ver2 + 0; // 1 2
        indices[tri+2] = ver2 + 1; // 3 4
        indices[tri+3] = ver1 + 0; // 1 2
        indices[tri+4] = ver1 + 1; // 2 3
        indices[tri+5] = ver2 + 1; // 3 4
      
        std::cout << "Vertices " << i << " " << vertices[top+0] << std::endl;
        std::cout << "Indices " << i << " " << indices[tri+0] << std::endl;


    };
    
    
    unsigned int VAO, VBO, EBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sides*8*2*sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sides*6*sizeof(float), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // enable
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians<float>(45.0), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    unsigned int modelLoc =glGetUniformLocation(ourShader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    // alternatively
    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    // ptr to the first byte of the matrix row col w/ a subscript operator
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    ourShader.setMat4("projection", projection);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        // render a blank win, goes first
        // r g b a 1:fully opaque
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // only clear the color buffer, not depth or stencil
        glClear(GL_COLOR_BUFFER_BIT);


        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(-.5f, .5f, 1.0f));
        // back reverse order, translate goes last
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        
        // compiling shader
        // glUseProgram(shaderProgram);
        // render container
        ourShader.use();
        
        
        unsigned int transformLoc =glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
         
         
        // handle for vertex array object vertex buffer
        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, sides*6, GL_UNSIGNED_INT, 0);

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
    // free memory, might get overridden by later, only when cleanup
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
        
}
