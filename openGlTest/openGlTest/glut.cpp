//
//  glut.cpp
//  openGlTest
//
//  Created by Stella on 9/25/22.
//

#include <iostream>

using namespace std;

#define GL_SILENCE_DEPRECATION

// #ifdef __APPLE__
// #include <GLUT/glut.h>
// #include <stdlib.h>
// #else
// #include <GL/glut.h>
// #endif

// #include <OpenGL/gl3.h>
// #define __gl_h_
// #include <GLUT/glut.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
/*
int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    return 0;
}
*/

void init(void)
{
   //Put OpenGL Initializing Code here

}

void display(void)
{
   //Put Drawing Code Here

   glutSwapBuffers();
}

void reshape (int w, int h)
{
   //Put Resizing Code Here

}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
      case 'q':
      case 'Q':
         exit(0);
         break;
   }
}

/*
int main(int argc, char** argv)
{
   glutInit(&argc, argv);

   //Select Pixel Format Attributes
   // glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_3_2_CORE_PROFILE);

   //Configure Window
   glutInitWindowSize (640, 480);
   glutInitWindowPosition (100, 100);

   //Create the Window and Set up Rendering Context
   glutCreateWindow (argv[0]);

   //Configure Rendering Context
   init ();

   //Connect callback functions that will respond to events
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);

   //Start listening for events
   glutMainLoop();
   return 0;
}
*/
