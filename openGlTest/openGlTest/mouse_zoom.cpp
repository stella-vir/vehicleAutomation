//
//  mouse_zoom.cpp
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

// cam
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
// facing
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

// field of view fov
// yaw .0f initialize a vetor to the right, start from 90
// orientation -> update front
float yaw = -90.0f;
float pitch = .0f;
// wheel
float fov = 45.0f;

// mouse state
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;

// time elapse since last frame
float deltaTime = 0.0f;    // Time between current frame and last frame
// timestamp of the previous frame
float lastFrame = 0.0f; // Time of last frame

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
    
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    // cross product
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// x y relative to prior val, one call to the next
// changes in x y val
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // start at true
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        // disable for the subsequent calls
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX; // 0
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // capped within -89-89 no backflip exceeding
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // vector length of 1 unit vector
    cameraFront = glm::normalize(front);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // ONLY within 1-45
    // scroll up zoom in decrease the fov
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
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
    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // mouse movement
    // cursor not visible, focus is on the window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
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
       
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
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
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        // next time around, will have it stored
        lastFrame = currentFrame;
        
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
  
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        ourShader.setMat4("projection", projection);
        
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        // handle for vertex array object vertex buffer
        glBindVertexArray(VAO);
        
        /*-------------------------------------------*/
        // array  need to change val as the vertices indices change
        // 36 instead of 6
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            // diff angles
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            
            // multiple draw calls
            // expensive -> instancing: diff model transforms
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        /*-------------------------------------------*/

        glfwSwapBuffers(window);
        // mouse handlers ev queue
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);


    glfwTerminate();
    return 0;
}



