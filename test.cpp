#define GLFW_INCLUDE_GLU

#include <GLFW/glfw3.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/key_codes.h>
#include <cstdlib> 
#include <iostream>
using namespace std;
//Initializes 3D rendering

//Initializes 3D rendering
void initializeRendering()
{
    glfwInit();
    //Makes 3D drawing work when something is in front of something else
    glEnable(GL_DEPTH_TEST);
}
 
//Called when a key is pressed

void display()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black
 
    //Clear information from last draw
    glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
 
    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
    glLoadIdentity(); //Reset the drawing perspective
 
    glBegin(GL_QUADS); //Begin quadrilateral coordinates
 
    //Trapezoid
    glVertex3f(-0.7f, -1.5f, -5.0f);
    glVertex3f(0.7f, -1.5f, -5.0f);
    glVertex3f(0.4f, -0.5f, -5.0f);
    glVertex3f(-0.4f, -0.5f, -5.0f);
 
    glEnd(); //End quadrilateral coordinates
 
    glBegin(GL_TRIANGLES); //Begin triangle coordinates
 
    //Pentagon
    glVertex3f(0.5f, 0.5f, -5.0f);
    glVertex3f(1.5f, 0.5f, -5.0f);
    glVertex3f(0.5f, 1.0f, -5.0f);
 
    glVertex3f(0.5f, 1.0f, -5.0f);
    glVertex3f(1.5f, 0.5f, -5.0f);
    glVertex3f(1.5f, 1.0f, -5.0f);
 
    glVertex3f(0.5f, 1.0f, -5.0f);
    glVertex3f(1.5f, 1.0f, -5.0f);
    glVertex3f(1.0f, 1.5f, -5.0f);
 
    //Triangle
    glVertex3f(-0.5f, 0.5f, -5.0f);
    glVertex3f(-1.0f, 1.5f, -5.0f);
    glVertex3f(-1.5f, 0.5f, -5.0f);
 
    glEnd(); //End triangle coordinates
    glfwSwapBuffers();
 
}
int main()
{
   // int     width, height;
    //int     frame = 0;
    bool    running = true;
 
    initializeRendering();
 
 
 

    glfwSetWindowTitle("codeincodeblock.blogspot.com - basic shape");
//callback function to handle keypress
    while(running) // infinite loop to draw object again and again
    {              // because once object is draw then window is terminated
        display();
        //running = glfwGetWindowParam( GLFW_OPENED ); //when glfw window is opened then it return true
                                                     //if closed then return false
    }
    return 0;
}
