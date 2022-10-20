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


class Vertices {
public:
    
    int sliceCt = 4;
    float radius = .5f;
    int baseCenterIndex;
    int topCenterIndex;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<float> interleavedVertices;

    
    // generate a unit circle on XY-plane
    std::vector<float> getUnitCircleVertices()
    {
        const float PI = 3.1415926f;
        float sectorStep = 2 * PI / sliceCt;
        float sectorAngle;  // radian

        std::vector<float> unitCircleVertices;
        for(int i = 0; i <= sliceCt; ++i)
        {
            sectorAngle = i * sectorStep;
            unitCircleVertices.push_back(cos(sectorAngle)); // x
            unitCircleVertices.push_back(sin(sectorAngle)); // y
            unitCircleVertices.push_back(0);                // z
        }
        return unitCircleVertices;
    };

    // generate vertices for a cylinder
    std::vector<float> buildVerticesSmooth()
    {
        // clear memory of prev arrays
        std::vector<float>().swap(vertices);
        std::vector<float>().swap(normals);
        std::vector<float>().swap(texCoords);
        
        
        // get unit circle vectors on XY-plane
        std::vector<float> unitVertices = getUnitCircleVertices();
        int height = 1;
        // put side vertices to arrays
        for(int i = 0; i < 2; ++i)
        {
            float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
            float t = 1.0f - i;                              // vertical tex coord; 1 to 0

            for(int j = 0, k = 0; j <= sliceCt; ++j, k += 3)
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
                texCoords.push_back((float)j / sliceCt); // s
                texCoords.push_back(t);                      // t
            }
        }

        // the starting index for the base/top surface
        //NOTE: it is used for generating indices later
        int baseCenterIndex = (int)vertices.size() / 3;
        int topCenterIndex = baseCenterIndex + sliceCt + 1; // include center vertex

        // put base and top vertices to arrays
        for(int i = 0; i < 2; ++i)
        {
            float h = -height / 2.0f + i * height;           // z value; -h/2 to h/2
            float nz = -1 + i * 2;                           // z value of normal; -1 to 1

            // center point
            vertices.push_back(0);     vertices.push_back(0);     vertices.push_back(h);
            normals.push_back(0);      normals.push_back(0);      normals.push_back(nz);
            texCoords.push_back(0.5f); texCoords.push_back(0.5f);

            for(int j = 0, k = 0; j < sliceCt; ++j, k += 3)
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
        return vertices;

    }

    std::vector<int> getIndices()
    {
        // generate CCW index list of cylinder triangles
        std::vector<int> indices;
        int k1 = 0;                         // 1st vertex index at base
        int k2 = sliceCt + 1;                // 1st vertex index at top

        // indices for the side surface
        for(int i = 0; i < sliceCt; ++i, ++k1, ++k2)
        {
            // 2 triangles per slice
            // k1 => k1+1 => k2
            indices.push_back(k1);
            indices.push_back(k1 + 1);
            indices.push_back(k2);

            // k2 => k1+1 => k2+1
            indices.push_back(k2);
            indices.push_back(k1 + 1);
            indices.push_back(k2 + 1);
        }

        // indices for the base surface
        //NOTE: baseCenterIndex and topCenterIndices are pre-computed during vertex generation
        //      please see the previous code snippet
        for(int i = 0, k = baseCenterIndex + 1; i < sliceCt; ++i, ++k)
        {
            if(i < sliceCt - 1)
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
        for(int i = 0, k = topCenterIndex + 1; i < sliceCt; ++i, ++k)
        {
            if(i < sliceCt - 1)
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
        return indices;
        
    }
    
    std::vector<float> buildInterleavedVertices()
    {
        std::vector<float>().swap(interleavedVertices);

        std::size_t i, j;
        std::size_t count = vertices.size();
        for(i = 0, j = 0; i < count; i += 3, j += 2)
        {
            //interleavedVertices.push_back(vertices[i]);
            //interleavedVertices.push_back(vertices[i+1]);
            //interleavedVertices.push_back(vertices[i+2]);
            interleavedVertices.insert(interleavedVertices.end(), &vertices[i], &vertices[i] + 3);

            //interleavedVertices.push_back(normals[i]);
            //interleavedVertices.push_back(normals[i+1]);
            //interleavedVertices.push_back(normals[i+2]);
            interleavedVertices.insert(interleavedVertices.end(), &normals[i], &normals[i] + 3);

            //interleavedVertices.push_back(texCoords[j]);
            //interleavedVertices.push_back(texCoords[j+1]);
            interleavedVertices.insert(interleavedVertices.end(), &texCoords[j], &texCoords[j] + 2);
        }
        return interleavedVertices;
    }
    
    
};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    Vertices ver;
    std::vector<float> res0 = ver.buildVerticesSmooth();
    std::vector<int> res = ver.getIndices();
    std::vector<float> res1 = ver.buildInterleavedVertices();

    for (int i=0; i<res0.size(); i++)
    {
        
        // std::cout << "Vertices" << res0[i]  << std::endl;
    }
    for (int j=0; j<res.size(); j++)
    {
        // std::cout << "Indices" << res[j] << std::endl;

    }
    
    // 60
    std::cout << "Vertices size" << res0.size()  << std::endl;
    // 48
    std::cout << "Indices size" << res.size() << std::endl;
    // 160
    std::cout << "Interleaved Vertices size" << res1.size()  << std::endl;




    return 0;
}



