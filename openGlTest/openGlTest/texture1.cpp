//
//  texture1.cpp
//  openGlTest
//
//  Created by Stella on 10/2/22.
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
#include <include/glad/glad.h>

// graphic library framework
// #include <GLFW/glfw3.h>
#include <include/GLFW/glfw3.h>
// #endif

#define STB_IMAGE_IMPLEMENTATION
// dependency
#include <include/stb_image.h>

// abstract
#include <include/filesystem.h>
// encapsulate the shader as a class
#include <include/shader_s.h>

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
// out  vec3: get passed down the pipeline
// aTexCord: vec2(aTexCoord.x, aTexCoord.y)
const char *vertexShaderSource = R"HERE(
    #version 330 core

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTexCoord;

    out vec3 ourColor;
    out vec2 TexCoord;


    void main()
    {
       gl_Position = vec4(aPos, 1.0);
       ourColor = aColor;
       TexCoord = aTexCoord;
    }
)HERE";


// first and only val, thus is the output of the pixel
// uniform global not allowed to change
// frame to frame/shader, change val to have color pulse
// fragment accept it as input of the color
// raspiration interpolation in each pixel of the shader

// sampler texture
// vertex shader var -> TexCoord u v
// sampler 2D: int val to repr texture mapping unit
// uniform sampler2D texture2;

// the 2nd contributes 20%, the first 80%
// FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
const char *fragmentShaderSource = R"HERE(
   #version 330 core
   out vec4 FragColor;

   in vec3 ourColor;
   in vec2 TexCoord;

   uniform sampler2D texture1;
   uniform sampler2D texture2;

   void main()
   {
       FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
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

    // shader class
    // initialize constructor pass in file path to the
    // vertex shader and fragment shader
    // boiler plates
    // OpenGL Shading Language Fragment Shader
    Shader ourShader("texture.vs", "texture1.fs");

    // int shaderProgram = glCreateProgram();
    /*
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
    */
    
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
        // pos            // colors rgb floats
        //  top right clockwise                // map to pos texture
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        // bottom right
        0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        // bottom left
        -0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        // top left
        -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    };

    // to draw rectangle out of these two triangles with the indices of the array
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // handle
    // element buffer object
    unsigned int VAO, VBO, EBO;
    // VAO max: 16 indices, how the data is laid-out in the buffer
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex array: indices ref to buffer
    // set the index and the nature of the data in the buffer
    // set index 0 and associate it to be bound with the current buffer
    // 3: vertices the cts from the elements in the buffer
    // false: not mormalized and supplies fixed vals
    // 3 * sizeof=12: the distance betweem the first byte of the one and the next byte of the other, skip over to the next attr of the same type
    // void* pointer offset/int: reading from the first element, not skipping any start/0/3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // enable
    glEnableVertexAttribArray(0);


    // color attrib 6 floats apart spaced
    // offset for the first color not 0
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // for texture attrib start from 2, counts of 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


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
    // set filtering params
    // minification magnification mipmap level
    // uv corrds don't correspond to the precise center of texcel to individual val
    // weighted avg of the immediate neigbours
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int width, height, nrChannels;
    // read files into an array of bytes
    // num of channels: 3 rgb
    // 512 * 512 size
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        // mipmap 0, internal format rgb
        // width of the boarder of the img 0, reading format from data: rgb
        // unsigned byte: the format of each val
        // GL_RGBA transparant bg pics
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
    // all upcoming 2D texture operations will have effect on this bound texture obj
    glBindTexture(GL_TEXTURE_2D, texture2);
    // config int val of the texture S T u v
    // repeat: wrapping pulse 1.1 ->.1 fractial components repeat
    // vertices 1.0f -> 2.0f stretch into twice the size
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set filtering params
    // minification magnification mipmap level
    // uv corrds don't correspond to the precise center of texcel to individual val
    // weighted avg of the immediate neigbours
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // read files into an array of bytes
    // num of channels: 3 rgb
    // 512 * 512 size
    // stbi_load(FileSystem::getPath("container1.jpg").c_str())
    unsigned char *data1 = stbi_load("container1.jpg", &width, &height, &nrChannels, 0);

    if (data1)
    {
        // mipmap 0, internal format rgb
        // width of the boarder of the img 0, reading format from data: rgb
        // unsigned byte: the format of each val
        // GL_RGBA ALPHA channel data reading from & texture writing into
        // transparant background pics
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
        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // float timeVal = glfwGetTime();
        // pulsing gradual transition [-1, 1]
        // float greenVal = sin(timeVal);
        // 4f: vector of the 4 floats after
        // function overloading
        // use the same shader program
        // glUniform4f(vertexColorLocation, 0.0f, greenVal, 0.0f, 1.0f);

        
        // make unit 0 the active texture and bind texture1
        // max: 16
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        // glBindTexture(GL_TEXTURE_2D, texture);

        // compiling shader
        // glUseProgram(shaderProgram);
        // render container
        ourShader.use();

        // handle for vertex array object vertex buffer
        glBindVertexArray(VAO);
        // use 3 vertices to draw the triangle, start from index 0
        // glDrawArrays(0) unbind

        // GL_ELEMENT_ARRAY_BUFFER
        // (primititives, no.of vertex indices, type of the indices,
        // offset-can set to read from other indices other than the first one)
        // no need for buffer, only the array, internal ref to any VBOs
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
    // glDeleteTextures(1, &texture);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);


    glfwTerminate();
    return 0;
}

