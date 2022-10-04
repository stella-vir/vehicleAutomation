//
//  vertices.h
//  openGlTest
//
//  Created by Stella on 10/3/22.
//

#ifndef vertices_h
#define vertices_h

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>


class Vertices {
public:
    
    // Vertices(int sliceCt = 4,
    //         float radius = .5f);
    // ~Vertices() {}
       

    // generate a unit circle on XY-plane
    std::vector<float> getUnitCircleVertices()
    {
        const float PI = 3.1415926f;
        float sliceStep = 2 * PI / sliceCt;
        float sliceAngle;  // radian

        std::vector<float> unitCircleVertices;
        for(int i = 0; i <= sliceCt; ++i)
        {
            sliceAngle = i * sliceStep;
            unitCircleVertices.push_back(cos(sliceAngle)); // x
            unitCircleVertices.push_back(sin(sliceAngle)); // y
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
        baseCenterIndex = (int)vertices.size() / 3;
        topCenterIndex = baseCenterIndex + sliceCt + 1; // include center vertex

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

    std::vector<unsigned int> getIndices()
    {
        // generate CCW index list of cylinder triangles
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
    
    void buildInterleavedVertices()
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
    }
    
    unsigned int getInterleavedVertexSize() const   { return (unsigned int)interleavedVertices.size() * sizeof(unsigned int);}    // # of bytes
    const float* getInterleavedVertices() const     { return &interleavedVertices[0]; }
    
    unsigned int getIndexSize() const       { return (unsigned int)indices.size() * sizeof(unsigned int); }
    const unsigned int* getIndices0() const  { return indices.data(); }

    int getInterleavedStride() const                { return interleavedStride; }   // should be 32 bytes

    unsigned int getIndexCount() const      { return (unsigned int)indices.size(); }

private:

    // member var
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<float> interleavedVertices;
    std::vector<unsigned int> indices;
    int interleavedStride = 32;                  // # of bytes to hop to the next vertex (should be 32 bytes)
    int sliceCt = 4;
    float radius = .5f;
    
    int baseCenterIndex;
    int topCenterIndex;
 
};

#endif /* vertices_h */
