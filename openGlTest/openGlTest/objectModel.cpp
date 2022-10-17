//
//  objectModel.cpp
//  openGlTest
//
//  Created by Stella on 10/4/22.
//

#include <stdio.h>
#include "objectModel.h"
#include <math.h>


#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <include/glad/glad.h>
#include <include/GLFW/glfw3.h>
#include <include/glm/glm.hpp>
#include <include/glm/gtc/matrix_transform.hpp>
#include <include/glm/gtc/type_ptr.hpp>
#include <include/shader_s.h>


const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 500;

// constructor
CObjectModel::CObjectModel():numVertices(0), numIndices(0), vertices(NULL), indices(NULL), VBO(0), EBO(0){
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
}

void CObjectModel::initialize(){

      

}




void CObjectModel::draw(float x, float y, float z, Shader* ourShader){
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, numVertices*5*sizeof(float), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(float), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // enable
    glEnableVertexAttribArray(0);
    // texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(.5f, 1.0f, 0.0f));
   // glm::mat4 model1 = glm::rotate(glm::mat4(1.0f), (float)0, glm::vec3(.5f, 0.0f, 0.0f));
    glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));

//        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    // glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // pointer ourShader ->
    unsigned int modelLoc =glGetUniformLocation(ourShader->ID, "model");
   // unsigned int viewLoc = glGetUniformLocation(ourShader->ID, "view");
    
    // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

    // alternatively

    // ptr to the first byte of the matrix row col w/ a subscript operator
    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    ourShader->setMat4("projection", projection);
     
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

CObjectModel::~CObjectModel(){
    delete vertices;
    delete indices;
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}


/*
CCylinder::CCylinder(float radius, float height, unsigned int sides):CObjectModel(){
    vertices = new float[5*2*sides];
    numVertices = 2*sides;
    indices = new unsigned int[6*sides];
    numIndices = 6*sides;
    float angle = (2.0f * M_PI )/ sides;
    
    
    for (int i=0; i<sides; i++)
    {
        int top = 5*2*i;
        int base = top+5;
        
        vertices[top+0] = cos(angle*i)*radius; // cos(2 * pi/10) * .3
        vertices[top+1] = height;
        vertices[top+2] = sin(angle*i)*radius;
        vertices[top+3] = (float)i / (float)sides;
        vertices[top+4] = 0.0f;
    
        vertices[base+0] = cos(angle*i)*radius;
        vertices[base+1] = -height;
        vertices[base+2] = sin(angle*i)*radius;
        vertices[base+3] = (float)i / (float)sides;
        vertices[base+4] = 1.0f;
        
       // std::cout << "Vertices" << i << " " << vertices[top+0] << " " << vertices[top+1] << " " << vertices[top+2] << std::endl;
            

        int tri = 6*i;
        int ver1 = 2*i;
        int ver2 = ver1 + 2;
        
        if (i == sides-1)
        {
            ver2 = 0;
        }
        
        indices[tri+0] = ver1 + 0; // 0
        indices[tri+1] = ver2 + 0; // 2
        indices[tri+2] = ver2 + 1; // 3
        indices[tri+3] = ver1 + 0; // 2
        indices[tri+4] = ver1 + 1; // 4
        indices[tri+5] = ver2 + 1; // 5
      
       // std::cout << "Indices" << i << " " << indices[tri+0] << " " << indices[tri+1] << " " << indices[tri+2] << std::endl;
        
    };
        
}
 */

CDisk::CDisk(float radius, float radius0, float height, float startAngle, float endAngle, unsigned int sides):CObjectModel(){
  
    bool angleIsFullCircle = (endAngle - startAngle) >= 360;

    if (angleIsFullCircle)
    {
        vertices = new float[5*2*sides];
        numVertices = 2*sides;
    }
    else
    {
        vertices = new float[5*2*(sides+1)];
        numVertices = 2*(sides+1);
    }
 
    indices = new unsigned int[6*sides];
    numIndices = 6*sides;
    
    // float angle = (2.0f * M_PI )/ sides;
    startAngle = (startAngle / 180) * M_PI;
    endAngle = (endAngle / 180) * M_PI;
    float angle = (endAngle - startAngle) / sides;
    
    if (!angleIsFullCircle)
    {
        sides += 1;
    }
    
    
    
    for (int i=0; i<sides; i++)
    {
        int top = 5*2*i;
        int base = top+5;
        
        vertices[top+0] = cos((angle*i)+startAngle)*radius; // cos(2 * pi/10) * .3
        vertices[top+1] = height;
        vertices[top+2] = sin((angle*i)+startAngle)*radius;
        vertices[top+3] = (float)i / (float)sides;
        vertices[top+4] = 0.0f;
    
        vertices[base+0] = cos((angle*i)+startAngle)*radius0;
        vertices[base+1] = height;
        vertices[base+2] = sin((angle*i)+startAngle)*radius0;
        vertices[base+3] = (float)i / (float)sides;
        vertices[base+4] = 1.0f;
        
       // std::cout << "Vertices" << i << " " << vertices[top+0] << " " << vertices[top+1] << " " << vertices[top+2] << std::endl;
            
        if (!angleIsFullCircle && i == sides - 1)
            break;
        
        int tri = 6*i;
        int ver1 = 2*i;
        int ver2 = ver1 + 2;
        
        if (i == sides-1)
        {
            ver2 = 0;
        }
        
        indices[tri+0] = ver1 + 0; // 0
        indices[tri+1] = ver2 + 0; // 2
        indices[tri+2] = ver2 + 1; // 3
        indices[tri+3] = ver1 + 0; // 2
        indices[tri+4] = ver1 + 1; // 4
        indices[tri+5] = ver2 + 1; // 5
      
       // std::cout << "Indices" << i << " " << indices[tri+0] << " " << indices[tri+1] << " " << indices[tri+2] << std::endl;
        
    };
 
}


CCylinder1::CCylinder1(float radius, float height, float startAngle, float endAngle, unsigned int sides):CObjectModel(){
    bool angleIsFullCircle = (endAngle - startAngle) >= 360;
    
    if (angleIsFullCircle) {
        vertices = new float[5*2*sides];
        numVertices = 2*sides;
    }
    else {
        vertices = new float[5*2*(sides+1)];
        numVertices = 2*(sides+1);
    }
    indices = new unsigned int[6*sides];
    numIndices = 6*sides;
    
    // float angle = (2.0f * M_PI )/ sides;
    startAngle = (startAngle / 180) * M_PI;
    endAngle = (endAngle / 180) * M_PI;
    float angle = (endAngle - startAngle) / sides;
    
    if (!angleIsFullCircle)
    {
        sides += 1;
    }
    
    for (int i=0; i<sides; i++)
    {
        int top = 5*2*i;
        int base = top+5;
        
        // vertices[top+0] = cos(angle*i)*radius; // cos(2 * pi/10) * .3
        
        // vertices[top+0] = cos(angle*i)*radius;
        vertices[top+0] = cos((angle*i)+startAngle)*radius; // cos(2 * pi/10) * .3
        vertices[top+1] = height;
        vertices[top+2] = sin((angle*i)+startAngle)*radius;
        vertices[top+3] = (float)i / (float)sides;
        vertices[top+4] = 0.0f;
    
        vertices[base+0] = cos((angle*i)+startAngle)*radius; // cos(2 * pi/10) * .3
        vertices[base+1] = -height;
        vertices[base+2] = sin((angle*i)+startAngle)*radius;
        vertices[base+3] = (float)i / (float)sides;
        vertices[base+4] = 1.0f;
        
       // std::cout << "Vertices" << i << " " << vertices[top+0] << " " << vertices[top+1] << " " << vertices[top+2] << std::endl;
            
        if (!angleIsFullCircle && i == sides - 1)
            break;
        
        int tri = 6*i;
        int ver1 = 2*i;
        int ver2 = ver1 + 2;
        
        if (i == sides-1)
        {
            ver2 = 0;
        }
        
        indices[tri+0] = ver1 + 0; // 0
        indices[tri+1] = ver2 + 0; // 2
        indices[tri+2] = ver2 + 1; // 3
        indices[tri+3] = ver1 + 0; // 2
        indices[tri+4] = ver1 + 1; // 4
        indices[tri+5] = ver2 + 1; // 5
      
       // std::cout << "Indices" << i << " " << indices[tri+0] << " " << indices[tri+1] << " " << indices[tri+2] << std::endl;
        
    };
        
}

