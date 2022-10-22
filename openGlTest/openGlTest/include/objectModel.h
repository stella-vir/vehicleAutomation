//
//  objectModel.h
//  openGlTest
//
//  Created by Stella on 10/4/22.
//

#ifndef objectModel_h
#define objectModel_h

#include <include/shader_s.h>

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

class CObjectModel {
public:
    CObjectModel ();
    // de-constructor
    ~CObjectModel();
    
    void initialize();
    // names named
    // float x, float y, float z, Shader* ourShader
    void draw(float, float, float, class Shader*);
    
    // no negative, only pos
    unsigned int numVertices;
    unsigned int numIndices;
    float *vertices;
    unsigned int *indices;
    unsigned int VBO;
    unsigned int EBO;
    
    
    
// protected:
    // mem var
    
    
// private:
    // mem var
    
};


class CCylinder:public CObjectModel {
public:
    // float radius, float height, unsigned int sides
    CCylinder(float, float, unsigned int);

};

class CCylinder1:public CObjectModel {
public:
    // float radius, float height, float startAngle, float endAngle, unsigned int sides
     CCylinder1(float, float, float, float, unsigned int);
};

class CDisk:public CObjectModel {
public:
    // float radius, float radius0, float height, float startAngle, float endAngle, unsigned int sides
    CDisk(float, float, float, float, float, unsigned int);
};

class CCircle:public CObjectModel {
public:
    CCircle(float radius, float height, unsigned int sides);
};

class CGear:public CObjectModel {
public:
    CGear(float radius, float toothRadius, float height, unsigned int sides, unsigned int protrudingSides);
};
class CGear1:public CObjectModel {
public:
    CGear1(float radius, float toothRadius, float height, unsigned int sides, unsigned int protrudingSides);
};



#endif /* objectModel_h */
