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

// constructor
CObjectModel::CObjectModel():numVertices(0), numIndices(0), vertices(NULL), indices(NULL), VBO(0), EBO(0){
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

CObjectModel::~CObjectModel(){
    delete vertices;
    delete indices;
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}

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
