//
//  cCylinder.cpp
//  openGlTest
//
//  Created by Stella on 10/12/22.
//


#include <stdio.h>
#include <iostream>
#include <math.h>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <include/glad/glad.h>
#include <include/GLFW/glfw3.h>
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
#include <include/vertices.h>
#include <include/objectModel.h>

//extern const unsigned int SCR_WIDTH;
//extern const unsigned int SCR_HEIGHT;

// cam
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
// facing
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 moveUp = glm::vec3(1.0f, 0.0f, 0.0f);


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
        // cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        cameraPos -= glm::normalize(glm::cross(cameraFront, moveUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        // cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        cameraPos += glm::normalize(glm::cross(cameraFront, moveUp)) * cameraSpeed;

}

void createObj(std::vector<CObjectModel*> &obj, float startAngle, float endAngle)
{
    //     CCylinder1 outCylinder1(.5f, .5f, 0, 120, 20);
    CCylinder1* outCylinder1 = new CCylinder1(.5f, .5f, startAngle, endAngle, 20);
    obj.push_back(outCylinder1);
    CCylinder1* outCylinder2 = new CCylinder1(.4f, .5f, startAngle, endAngle, 20);
    obj.push_back(outCylinder2);

    // CDisk* topDisk = new CDisk(.5f, .4f, .5f, 0, 120, 20);
    CDisk* topDisk = new CDisk(.5f, .4f, .5f, startAngle, endAngle, 20);
    obj.push_back(topDisk);

    CDisk* baseDisk = new CDisk(.5f, .4f, -.5f, startAngle, endAngle, 20);
    obj.push_back(baseDisk);

    // delete
}

void deleteObj(std::vector<CObjectModel*> &obj)
{
    for (int i=0; i<obj.size(); i++)
    {
        delete obj[i];
    }
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
    
    // set the window
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
    
    // the farthest object being drawn first, no it won't overlap on the top of the near oject
    glEnable(GL_DEPTH_TEST);
    
    // complile loc: schema
    Shader ourShader("cylinder.vs", "cylinder.fs");

    
    /*-----------------------------*/
    /*
    float vertices[] = {
    // clockwise
     //  top right       // colors rgb       // map to pos texture
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     // bottom right
     0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
     // bottom left
     -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
     // top left
     -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    };
    // for rectangle connect every three vertices to form triangles
    float indices[] = {
     0, 1, 3,
     1, 2, 3
    };
     */
    
    // CCylinder inCylinder(.2f, .8f, 20);
    CCylinder1 inCylinder1(.2f, .8f, 0, 360, 20);

    std::vector<CObjectModel*> obj;
    std::vector<CObjectModel*> obj1;
    std::vector<CObjectModel*> obj2;

    createObj(obj, 0, 120);
    createObj(obj1, 120, 240);
    createObj(obj2, 240, 360);

    /*
//    CCylinder outCylinder(.5f, .5f, 20);
    CCylinder1 outCylinder1(.5f, .5f, 0, 120, 20);
    obj.push_back(&outCylinder1);
    CCylinder1 outCylinder2(.4f, .5f, 0, 120, 20);
    obj.push_back(&outCylinder2);


    CDisk topDisk(.5f, .4f, .5f, 0, 120, 20);
    obj.push_back(&topDisk);

    CDisk baseDisk(.5f, .4f, -.5f, 0, 120, 20);
    obj.push_back(&baseDisk);

    */

    /*-----------------------------*/


    // Vertices ver;
    // unsigned int vertexSize = ver.getInterleavedVertexSize();
    // unsigned int vertexSize = 160;

   //  unsigned int indexSize = ver.getIndexSize();
    // unsigned int indexSize = 48;

    // unsigned int VAO, VBO, EBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);

    /*
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // sides*8*2*sizeof(float) // sizeof(vertices) // vertexSize
    // ver.getInterleavedVertices()

    glBufferData(GL_ARRAY_BUFFER, sides*5*2*sizeof(float), vertices, GL_STATIC_DRAW);


   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // sides*6*sizeof(float) // sizeof(indices) indexSize
    // ver.getIndices0()
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sides*6*sizeof(float), indices, GL_STATIC_DRAW);
    */
    
    /*-------------------------------------------*/
    // 5 * sizeof(float)
    // 4 bytes every 3 coords/floats = 12
    // int stride = ver.getInterleavedStride();
    
    // vertex, need to change val as the vertices indices change
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    // enable
//    glEnableVertexAttribArray(0);
//
    // normal
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
    
//    // texture
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
    /*-------------------------------------------*/

    
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("metal-container.jpg", &width, &height, &nrChannels, 0);

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

    // de-allocate, data has been copied to texture obj
    stbi_image_free(data);

    // texture object 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // config int val of the texture S T u v
    // repeat: wrapping pulse 1.1 ->.1 fractial components repeat
    // vertices 1.0f -> 2.0f stretch into twice the size
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    unsigned char *data1 = stbi_load("metal-container.jpg", &width, &height, &nrChannels, 0);


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
    // de-allocate, data has been copied to texture obj
    stbi_image_free(data1);
    
    
    // activate first
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
        glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
        // only clear the color buffer, not depth or stencil
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        ourShader.use();
        
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);
        
        // float pos = cos(glfwGetTime()*4)*.5f;
        float pos = cos(glfwGetTime()*4)*.5f + .5f;

        
//        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(.5f, 1.0f, 0.0f));
////        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
//        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f, -3.0f));
//        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//
//        unsigned int modelLoc =glGetUniformLocation(ourShader.ID, "model");
//        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
//
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        // alternatively
//
//        // ptr to the first byte of the matrix row col w/ a subscript operator
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
//
//        ourShader.setMat4("projection", projection);
//
        // handle for vertex array object vertex buffer
        glBindVertexArray(VAO);
        
        /*-------------------------------------------*/
        // array  need to change val as the vertices indices change
        // 36 -> ?48 72 // sides*6 // ver 60 idx 48
        // ver.getIndexCount()
        // (void*)0 0
        
        inCylinder1.draw(.0f, .0f, -3.0f, &ourShader);

        
        // cos and sin of 120 / 2 = 60, -120=300, -120=180
        float c = cos((60.f / 180.f) * M_PI);
        float s = sin((60.f / 180.f) * M_PI);

        float c1 = cos((300.f / 180.f) * M_PI);
        float s1 = sin((300.f / 180.f) * M_PI);

        float c2 = cos((180.f / 180.f) * M_PI);
        float s2 = sin((180.f / 180.f) * M_PI);

        /*
        // outCylinder.draw();
       // outCylinder1.draw(pos, .0f, -3.0f, &ourShader);
//        outCylinder1.draw(pos*s, .0f, -3.0f+pos*c, &ourShader);
        //outCylinder2.draw(pos, .0f, -3.0f, &ourShader);
        outCylinder2.draw(pos*s, .0f, -3.0f+pos*c, &ourShader);

       // topDisk.draw(pos, .0f, -3.0f, &ourShader);
        topDisk.draw(pos*c, .0f, -3.0f+pos*s, &ourShader);

        // baseDisk.draw(pos, .0f, -3.0f, &ourShader);
        baseDisk.draw(pos*c, .0f, -3.0f+pos*s, &ourShader);
//        glDrawElements(GL_TRIANGLES, sides*6, GL_UNSIGNED_INT, 0);
         */
        
        for (int i=0; i<obj.size(); i++)
        {
            obj[i]->draw(pos*s, .0f, -3.0f+pos*c, &ourShader);
        }
        
        for (int i=0; i<obj1.size(); i++)
        {
            obj1[i]->draw(pos*s1, .0f, -3.0f+pos*c1, &ourShader);
        }
        
        for (int i=0; i<obj2.size(); i++)
        {
            obj2[i]->draw(pos*s2, .0f, -3.0f+pos*c2, &ourShader);
        }
        /*-------------------------------------------*/

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

    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

    // unbind buffer
   // glBindBuffer(GL_ARRAY_BUFFER, 0);
   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    deleteObj(obj);
    deleteObj(obj1);
    deleteObj(obj2);

    glfwTerminate();
    return 0;
        
}
