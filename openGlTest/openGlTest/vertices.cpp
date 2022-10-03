//
//  vertices.cpp
//  openGlTest
//
//  Created by Stella on 10/3/22.
//

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>

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


// generate CCW index list of cylinder triangles
std::vector<int> indices;
int k1 = 0;                         // 1st vertex index at base
int k2 = sectorCount + 1;           // 1st vertex index at top

// indices for the side surface
for(int i = 0; i < sectorCount; ++i, ++k1, ++k2)
{
    // 2 triangles per sector
    // k1 => k1+1 => k2
    indices.push_back(k1);
    indices.push_back(k1 + 1);
    indices.push_back(k2);

    // k2 => k1+1 => k2+1
    indices.push_back(k2);
    indices.push_back(k1 + 1);
    indices.push_back(k2 + 1);
};

// indices for the base surface
//NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
//      please see the previous code snippet
for(int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
{
    if(i < sectorCount - 1)
    {
        indices.push_back(baseCenterIndex);
        indices.push_back(k + 1);
        indices.push_back(k);
    }
    else // last triangle
    {
        indices.push_back(baseCenterIndex);
        indices.push_back(baseCenterIndex + 1);
        indices.push_back(k);
    }
}

// indices for the top surface
for(int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
{
    if(i < sectorCount - 1)
    {
        indices.push_back(topCenterIndex);
        indices.push_back(k);
        indices.push_back(k + 1);
    }
    else // last triangle
    {
        indices.push_back(topCenterIndex);
        indices.push_back(k);
        indices.push_back(topCenterIndex + 1);
    }
}




// generate a unit circle on XY-plane
std::vector<float> Cylinder::getUnitCircleVertices()
{
    const float PI = 3.1415926f;
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;  // radian

    std::vector<float> unitCircleVertices;
    for(int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }
    return unitCircleVertices;
};

// generate vertices for a cylinder
void Cylinder::buildVerticesSmooth()
{
    // clear memory of prev arrays
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);

    // get unit circle vectors on XY-plane
    std::vector<float> unitVertices = getUnitCircleVertices();

    // put side vertices to arrays
    for(int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
        float t = 1.0f - i;                              // vertical tex coord; 1 to 0

        for(int j = 0, k = 0; j <= sectorCount; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k+1];
            float uz = unitVertices[k+2];
            // position vector
            vertices.push_back(ux * radius);             // vx
            vertices.push_back(uy * radius);             // vy
            vertices.push_back(h);                       // vz
            // normal vector
            normals.push_back(ux);                       // nx
            normals.push_back(uy);                       // ny
            normals.push_back(uz);                       // nz
            // texture coordinate
            texCoords.push_back((float)j / sectorCount); // s
            texCoords.push_back(t);                      // t
        }
    }

    // the starting index for the base/top surface
    //NOTE: it is used for generating indices later
    int baseCenterIndex = (int)vertices.size() / 3;
    int topCenterIndex = baseCenterIndex + sectorCount + 1; // include center vertex

    // put base and top vertices to arrays
    for(int i = 0; i < 2; ++i)
    {
        float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
        float nz = -1 + i * 2;                           // z value of normal; -1 to 1

        // center point
        vertices.push_back(0);     vertices.push_back(0);     vertices.push_back(h);
        normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
        texCoords.push_back(0.5f); texCoords.push_back(0.5f);

        for(int j = 0, k = 0; j < sectorCount; ++j, k += 3)
        {
            float ux = unitVertices[k];
            float uy = unitVertices[k+1];
            // position vector
            vertices.push_back(ux * radius);             // vx
            vertices.push_back(uy * radius);             // vy
            vertices.push_back(h);                       // vz
            // normal vector
            normals.push_back(0);                        // nx
            normals.push_back(0);                        // ny
            normals.push_back(nz);                       // nz
            // texture coordinate
            texCoords.push_back(-ux * 0.5f + 0.5f);      // s
            texCoords.push_back(-uy * 0.5f + 0.5f);      // t
        }
    }
}
