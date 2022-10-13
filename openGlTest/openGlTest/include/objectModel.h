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

/*
class CCylinder:public CObjectModel {
public:
    // CCylinder(float, float, unsigned int);
     CCylinder1(float, float, float, float, unsigned int);
};
*/
class CCylinder1:public CObjectModel {
public:
    // CCylinder(float, float, unsigned int);
     CCylinder1(float, float, float, float, unsigned int);
};

class CDisk:public CObjectModel {
public:
    CDisk(float, float, float, float, float, unsigned int);
};


#endif /* objectModel_h */
