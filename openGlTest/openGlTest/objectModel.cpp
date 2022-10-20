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
    // size of the vertices
    glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(float), vertices, GL_STATIC_DRAW);
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
    
//        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // pointer ourShader ->
    unsigned int modelLoc =glGetUniformLocation(ourShader->ID, "model");
     unsigned int viewLoc = glGetUniformLocation(ourShader->ID, "view");
    
    /*-------------------------------------------*/
    // model view --need to change as to put view-translate into model-rotate mat4 loc, no rotation

//     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    // alternatively
    // ptr to the first byte of the matrix row col w/ a subscript operator
//     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));
    /*-------------------------------------------*/
   

    ourShader->setMat4("projection", projection);
     
    /*-------------------------------------------*/
    // array --need to change val as the vertices/indices change
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    /*-------------------------------------------*/

}

// deconstructor
CObjectModel::~CObjectModel(){
    delete vertices;
    delete indices;
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}

// 120 degree 3 phrases motor
CCylinder1::CCylinder1(float radius, float height, float startAngle, float endAngle, unsigned int sides):CObjectModel(){
    bool angleIsFullCircle = (endAngle - startAngle) >= 360;
    
    if (angleIsFullCircle) {
        vertices = new float[5*2*sides];
        numVertices = 5*2*sides;
    }
    else {
        vertices = new float[5*2*(sides+1)];
        numVertices = 5*2*(sides+1);
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



// a whole cylinder
CCylinder::CCylinder(float radius, float height, unsigned int sides):CObjectModel(){
    vertices = new float[5*2*sides];
    numVertices = 5*2*sides;
    indices = new unsigned int[6*sides];
    numIndices = 6*sides;
    float angle = (2.0f * M_PI )/ sides;
    
    
    for (int i=0; i<sides; i++)
    {
        int top = 5*2*i;
        int base = top+5;
        
        vertices[top+0] = cos(angle*i)*radius; // cos(2 * pi/10) * .3
        vertices[top+1] = sin(angle*i)*radius;
        vertices[top+2] = height;
        vertices[top+3] = (float)i / (float)sides;
        vertices[top+4] = 0.0f;
    
        vertices[base+0] = cos(angle*i)*radius;
        vertices[base+1] = sin(angle*i)*radius;
        vertices[base+2] = -height;
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


CDisk::CDisk(float radius, float radius0, float height, float startAngle, float endAngle, unsigned int sides):CObjectModel(){
  
    bool angleIsFullCircle = (endAngle - startAngle) >= 360;

    if (angleIsFullCircle)
    {
        vertices = new float[5*2*sides];
        numVertices = 5*2*sides;
    }
    else
    {
        vertices = new float[5*2*(sides+1)];
        numVertices = 5*2*(sides+1);
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
        
//        std::cout << "Vertices" << i << " " << vertices[top+0] << " " << vertices[top+1] << " " << vertices[top+2] << std::endl;
            
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
      
//        std::cout << "Indices" << i << " " << indices[tri+0] << " " << indices[tri+1] << " " << indices[tri+2] << std::endl;
        
    };
 
}

CCircle::CCircle(float radius, float height, unsigned int sides) {
    vertices = new float[5*2*sides];
    numVertices = 5*2*sides;

    indices = new unsigned int[6 * sides];
    numIndices = 6*sides;

    
    float angle = (2.0f * M_PI) / sides ;
    
    // 0 1 2 3
    for (int i=0; i<sides; i++)
    {
        int top = 5 * 2 * i;
        int base = top+5;
        
        vertices[top+0] = cos(angle * i) * radius;
        vertices[top+1] = sin(angle * i) * radius;
        vertices[top+2] = height;
        // texture for the sides
        // u: 1/sides, 2/sides...portions
        // v: 0-1 top-to-bot
        vertices[top+3] = (float)i / (float)sides;
        vertices[top+4] = 0.0f;
        
        vertices[base+0] = cos(angle * i) * radius;
        vertices[base+1] = sin(angle * i) * radius;
        vertices[base+2] = -height;
        vertices[base+3] = (float)i / (float)sides;
        vertices[base+4] = 1.0f;
        
        std::cout << "Vertices" << " " << i << " " << angle << " Top: " << vertices[top+0] << " " << vertices[top+1] << " " << vertices[top+2] << " " << vertices[top+3] << " " << vertices[top+4] << " Base: " << vertices[base+0] << " " << vertices[base+1] << " " << vertices[base+2] << " " << vertices[base+3] << " " << vertices[base+4] << std::endl;
        
        // indices: every 6th of them for each side
//        unsigned int indices[] = {
//            0, 1, 3,
//            1, 2, 3
//        };
        
        int tri = 6 * i;
        // only increment either 0 or 1
        // *2: i, i+1, i+3, i+1, i+2, i+3 -> jump to n + 1
        int ver1 = 2 * i;
        int ver2 = ver1 + 2;
        
        if (i == sides-1)
        {
            ver2 = 0;
        }

        indices[tri+0] = ver1 + 0;
        indices[tri+1] = ver1 + 1;
        indices[tri+2] = ver2 + 0;
        indices[tri+3] = ver1 + 1;
        indices[tri+4] = ver2 + 0;
        indices[tri+5] = ver2 + 1;
        
        // half of the circle *2: jump to n + 1
//        indices[tri+0] = i;
//        indices[tri+1] = i + 1;
//        indices[tri+2] = i + 3;
//        indices[tri+3] = i + 1;
//        indices[tri+4] = i + 2;
//        indices[tri+5] = i + 3;
        
    }
}

CGear::CGear(float radius, float toothRadius, float height, unsigned int sides, unsigned int protrudingSides):CObjectModel(){
  
    unsigned int s = sides * protrudingSides;

    // 5: (x, y, z, u, v)
    vertices = new float[5*2*s];
    numVertices = 5*2*s;

    // 6: 2 triangles * 3 (top + base + side)
    indices = new unsigned int[6*s];
    numIndices = 6*s;
    
    float angle = (2.0f * M_PI) / s;
    // the protruding parts, divisible by 4
    float protrudingRadius = radius + toothRadius;
 
// you need to believe deep in your heart that you can do it.
    // 0 1 2 3
    for (int i=0; i<s; i++)
    {
        int top = 5 * 2 * i;
        int base = top+5;
        std::cout << "i" << " " << i << std::endl;
//        float intermittentAngle = (angle * i) - (angle / protrudingSides) * (protrudingSides - 1);
//        float intermittentAngle1 = (angle * i) - (angle / protrudingSides) * (protrudingSides - 2);
        
        
        if (i % 3 == 0)
        {
            vertices[top+0] = cos(angle * i) * radius;
            vertices[top+1] = sin(angle * i) * radius;
            vertices[top+2] = height;
            // texture for the sides
            // u: 1/sides, 2/sides...portions
            // v: 0-1 top-to-bot
            vertices[top+3] = (float)i / (float)s;
            vertices[top+4] = 0.0f;
            
            vertices[base+0] = cos(angle * i) * radius;
            vertices[base+1] = sin(angle * i) * radius;
            vertices[base+2] = -height;
            vertices[base+3] = (float)i / (float)s;
            vertices[base+4] = 1.0f;
        }
        
        else {
            vertices[top+0] = cos(angle * i) * protrudingRadius;
            vertices[top+1] = sin(angle * i) * protrudingRadius;
            vertices[top+2] = height;
            // texture for the sides
            // u: 1/sides, 2/sides...portions
            // v: 0-1 top-to-bot
            vertices[top+3] = (float)i / (float)s;
            vertices[top+4] = 0.0f;
            
            vertices[base+0] = cos(angle * i) * protrudingRadius;
            vertices[base+1] = sin(angle * i) * protrudingRadius;
            vertices[base+2] = -height;
            vertices[base+3] = (float)i / (float)s;
            vertices[base+4] = 1.0f;
        }
        
        

//
        
        std::cout << "Vertices" << " Angle: " << angle << " Top: " << vertices[top+0] << " " << vertices[top+1] << " " << vertices[top+2] << " " << vertices[top+3] << " " << vertices[top+4] << " Base: " << vertices[base+0] << " " << vertices[base+1] << " " << vertices[base+2] << " " << vertices[base+3] << " " << vertices[base+4] << std::endl;
        
        // indices: every 6th of them for each side
        // it goes counter clockwise
//        unsigned int indices[] = {
//            0, 1, 3,
//            1, 2, 3
//        };
        
        int tri = 6 * i;
        // only increment either 0 or 1
        // *2: i, i+1, i+3, i+1, i+2, i+3 -> jump to n + 1
        int ver1 = 2 * i;
        int ver2 = ver1 + 2;
        
        if (i == s-1)
        {
            ver2 = 0;
        }

        indices[tri+0] = ver1 + 0;
        indices[tri+1] = ver1 + 1;
        indices[tri+2] = ver2 + 0;
        indices[tri+3] = ver1 + 1;
        indices[tri+4] = ver2 + 0;
        indices[tri+5] = ver2 + 1;
        
        // half of the circle *2: jump to n + 1
//        indices[tri+0] = i;
//        indices[tri+1] = i + 1;
//        indices[tri+2] = i + 3;
//        indices[tri+3] = i + 1;
//        indices[tri+4] = i + 2;
//        indices[tri+5] = i + 3;
        
    }
 
};
