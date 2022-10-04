//
//  objectModel.h
//  openGlTest
//
//  Created by Stella on 10/4/22.
//

#ifndef objectModel_h
#define objectModel_h

class CObjectModel {
public:
    CObjectModel ();
    // de-constructor
    ~CObjectModel();
    
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
    CCylinder(float, float, unsigned int );
};

#endif /* objectModel_h */
