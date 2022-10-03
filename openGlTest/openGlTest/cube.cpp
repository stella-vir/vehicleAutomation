//
//  cube.cpp
//  openGlTest
//
//  Created by Stella on 10/3/22.
//

#include <stdio.h>
#include <iostream>
#include <math.h>

#define GL_SILENCE_DEPRECATION

#define GLFW_INCLUDE_NONE
#include <include/glad/glad.h>
#include <include/GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <include/stb_image.h>
#include <include/glm/glm.hpp>
#include <include/glm/gtc/matrix_transform.hpp>
#include <include/glm/gtc/type_ptr.hpp>
#include <include/filesystem.h>
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // monitor mode & global state
        glfwTerminate();
        return -1;
    }
    
    // primitives are rendered the last, on the top when it should be appearing in the back behind if not enabled
    glEnable(GL_DEPTH_TEST);
    
    Shader ourShader("projection.vs", "texture1.fs");
    
    float vertices[] = {
        // counter-clockwise
        // bot left       // map texture
        -.5f, -.5f, -.5f,  .0f, .0f,
        // bottom right
        .5f, -.5f, -.5f,  1.0f, .0f,
        .5f, .5f, -.5f,  1.0f, 1.0f,
        .5f, .5f, -.5f,  1.0f, 1.0f,
        -.5f, .5f, -.5f, .0f, 1.0f,
        -.5f, -.5f, -.5f, .0f, .0f,
        
        // bot left       // map texture
        -.5f, -.5f, .5f,  .0f, .0f,
        // bottom right
        .5f, -.5f, .5f,  1.0f, .0f,
        .5f, .5f, .5f,  1.0f, 1.0f,
        .5f, .5f, .5f,  1.0f, 1.0f,
        -.5f, .5f, .5f, .0f, 1.0f,
        -.5f, -.5f, .5f, .0f, .0f,
        
        // top left       // map texture
        -.5f, .5f, .5f,  1.0f, .0f,
        // deep top left
        -.5f, .5f, -.5f,  1.0f, 1.0f,
        // deep top right
        -.5f, -.5f, -.5f,  .0f, 1.0f,
        -.5f, -.5f, -.5f,  .0f, 1.0f,
        -.5f, -.5f, .5f,  .0f, .0f,
        -.5f, .5f, .5f, 1.0f, .0f,
        
        // top left       // map texture
        .5f, .5f, .5f,  1.0f, .0f,
        // deep top left
        .5f, .5f, -.5f,  1.0f, 1.0f,
        // deep top right
        .5f, -.5f, -.5f,  .0f, 1.0f,
        .5f, -.5f, -.5f,  .0f, 1.0f,
        .5f, -.5f, .5f,  .0f, .0f,
        .5f, .5f, .5f, 1.0f, .0f,
        
        
        -.5f, -.5f, -.5f,  .0f, 1.0f,
        .5f, -.5f, -.5f,  1.0f, 1.0f,
        .5f, -.5f, .5f,  1.0f, .0f,
        .5f, -.5f, .5f,  1.0f, .0f,
        -.5f, -.5f, .5f, .0f, .0f,
        -.5f, -.5f, -.5f, .0f, 1.0f,
        
        -.5f, .5f, -.5f,  .0f, 1.0f,
        .5f, .5f, -.5f,  1.0f, 1.0f,
        .5f, .5f, .5f,  1.0f, .0f,
        .5f, .5f, .5f,  1.0f, .0f,
        -.5f, .5f, .5f, .0f, .0f,
        -.5f, .5f, -.5f, .0f, 1.0f,
        
        
    };
 
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    

    /*-------------------------------------------*/
    // vertex, need to change val as the vertices indices change
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // texture, need to change val as the vertices indices change
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    /*-------------------------------------------*/

    
    // texture object 1
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    // all upcoming 2D texture operations will have effect on this bound texture obj
    glBindTexture(GL_TEXTURE_2D, texture1);
    // config int val of the texture S T u v
    // repeat: wrapping pulse 1.1 ->.1 fractial components repeat
    // vertices 1.0f -> 2.0f stretch into twice the size
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    // flip
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // gen diff levels of minmaps
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    // texture obj 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data1 = stbi_load("container1.jpg", &width, &height, &nrChannels, 0);
    
    if (data1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        // gen diff levels of minmaps
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data1);
    
    ourShader.use();
    // set sampler2D vals to 0 1 explicitly
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    ourShader.setInt("texture2", 1);
    

    
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        // render a blank win, goes first
        // r g b a 1:fully opaque
        glClearColor(0.3f, 0.3f, 0.5f, .8f);
        // clear the color buffer, AND the depth or stencil
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        ourShader.use();
      
        // rotate not around one of the three cardinal axes, seems tumble
        glm::mat4 model = glm::rotate(glm::mat4(1.0f),(float)glfwGetTime(), glm::vec3(.5f, 1.0f, 0.0f));
        // view move the camera backwards starting from the xy plane, -z
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        unsigned int modelLoc =glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // alternatively
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        
        ourShader.setMat4("projection", projection);
        
        
        // handle for vertex array object vertex buffer
        glBindVertexArray(VAO);
        
        /*-------------------------------------------*/
        // array  need to change val as the vertices indices change
        // 36 instead of 6
        glDrawArrays(GL_TRIANGLES, 0, 36);        
        /*-------------------------------------------*/

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);


    glfwTerminate();
    return 0;
}
