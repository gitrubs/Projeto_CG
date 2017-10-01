//
//  main.cpp
//  Projeto_CG
//
//  Created by Rubens Gondek on 10/1/17.
//  Copyright © 2017 gondek. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <math.h>
#include <iostream>
#include <fstream>

void myIdle();
void myDisplay();

struct TVector
{
    float x,y,z;
};

struct TObject3D
{
    TVector position;
    TVector velocity;
};

TObject3D sphere;

#define GRAVITY 4.0

void RunPhysics(float dt)
{
    sphere.velocity.y=sphere.velocity.y-GRAVITY*dt;
    
    sphere.position.x=sphere.position.x+sphere.velocity.x*dt;
    sphere.position.y=sphere.position.y+sphere.velocity.y*dt;
    sphere.position.z=sphere.position.z+sphere.velocity.z*dt;
    
    if (sphere.velocity.y >= 0) return;
    
    if (sphere.position.y < 0) {
        if ((sphere.position.x>-2) && (sphere.position.x<-1.2)) {
            sphere.velocity.y=-sphere.velocity.y;
        }
    }
    
    if (sphere.position.y<-0) sphere.velocity.y=-sphere.velocity.y;
    
}

void myInit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 64/48.0, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    glColor3d(0.9,0.1,0.3);
}

void myIdle()
{
    RunPhysics(0.01);
    myDisplay();
    
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix(); // draw and place Sphere
    glTranslated(sphere.position.x,sphere.position.y,sphere.position.z);
    glutSolidSphere(0.2, 15, 15);
    glPopMatrix();
    
    
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    sphere.position.x=0;
    sphere.position.y=1;
    sphere.position.z=0;
    
    sphere.velocity.x=0;
    sphere.velocity.y=0;
    sphere.velocity.z=0;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowSize(640,480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Rolling");
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);
    glClearColor(1.0f, 1.0f, 1.0f,0.0f);
    glViewport(0, 0, 640, 480);
    //glutKeyboardFunc(myKeyboard);
    myInit();
    glutMainLoop();
    myIdle();
    
    return 0;
}
