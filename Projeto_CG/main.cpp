//
//  main.cpp
//  Projeto_CG
//
//  Created by Rubens Gondek - 41411978
//  Created by Rodrigo Friedrich - 31409490
//  Copyright © 2017 gondek. All rights reserved.
//

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "SOIL.h"

//GAP do contador entre carregamentos
#define NUMBERGAP 250
#define OPGAP 250
#define GAP 50
#define SCORE_TO_WIN 5
#define BG_SIZE 450

GLuint texture_id[1];

static int sPosy = 100, bPosy = 0;
static int numbersX[6] = {50, 100, 150, 200, 250, 300};
static int backgroundX1 = -50, backgroundX2 = 250;
static int number[6] = {1, 2, 3, 4, 5, 6};
static int opX[4] = {75, 150, 225, 300};
static char op[4] = {'+', '-', '*', '/'};

static int n1 = -1, n2 = -1;
static char currentOp = ' ';
static int currentTarget = 4;
static char currentStatus[150] = ";";

static int started = 0;

static int score = 0;
static int won = 0;

static GLfloat ambient1[4]={0.2,0.2,0.2,1.0};
static GLfloat difuse1[4]={0.7,0.7,0.7,1.0};
static GLfloat especular1[4]={1.0, 1.0, 1.0, 1.0};
static GLfloat position1[4]={10.0, 50.0, 0.0, 1.0};

static GLfloat focal2[4]={0.2,0.2,0.2,1.0};
static GLfloat difuse2[4]={0.7,0.7,0.7,1.0};
static GLfloat especular2[4]={1.0, 1.0, 1.0, 1.0};
static GLfloat position2[4]={0.0, 100.0, 50.0, 1.0};

static GLfloat reflex[4]={1.0,1.0,1.0,1.0};
static GLint objReflex = 60;

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glShadeModel(GL_FLAT);
    
    glGenTextures (1, texture_id);  // 1 = uma textura;
    // texture_id = vetor que guarda os números das texturas
    
    // Carrega um arquivo de imagem diretamente como uma nova textura OpenGL
    texture_id[0] = SOIL_load_OGL_texture
    (
     "bg.png",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
     );
    
    /* check for an error during the load process */
    if( 0 == texture_id[0] )
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
}

void drawBitmapText(char * string, float x, float y, float z) {
    glDisable(GL_LIGHTING);
    
    char * c;
    
    glRasterPos3f(x, y, z);
    
    for (c = string; * c != ';'; c++) {
        
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, * c);
    }
    
    glEnable(GL_LIGHTING);
}

void background() {
    glDisable(GL_LIGHTING);

    glEnable ( GL_TEXTURE_2D );
    glBindTexture(GL_TEXTURE_2D, texture_id[0]);
    glBegin (GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glTexCoord2d(0, 0); glVertex3f(backgroundX1,-15.0,0.0);
    glTexCoord2d(1, 0); glVertex3f(backgroundX1+BG_SIZE,-15.0,0.0);
    glTexCoord2d(1, 1); glVertex3f(backgroundX1+BG_SIZE,115.0,0.0);
    glTexCoord2d(0, 1); glVertex3f(backgroundX1,115.0,0.0);
    glEnd();
    glBegin (GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glTexCoord2d(0, 0); glVertex3f(backgroundX2,-15.0,0.0);
    glTexCoord2d(1, 0); glVertex3f(backgroundX2+BG_SIZE,-15.0,0.0);
    glTexCoord2d(1, 1); glVertex3f(backgroundX2+BG_SIZE,115.0,0.0);
    glTexCoord2d(0, 1); glVertex3f(backgroundX2,115.0,0.0);
    glEnd();
    glDisable ( GL_TEXTURE_2D );
    glEnable(GL_LIGHTING);
}

void floor() {
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(1, 0.5, 0.5);
    glVertex3i(0, 100, 0);
    glVertex3i(300, 100, 0);
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(0.5, 1, 1);
    glVertex3i(0, 0, 0);
    glVertex3i(300, 0, 0);
    glEnd();
    glPopMatrix();
}

void bouncy() {
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslatef(15, bPosy, 0);
    glutSolidSphere(5, 10, 10);
    glPopMatrix();
}

void swingy() {
    glColor3f(0.2, 0.0, 0.3);
    
    glPushMatrix();
    glTranslatef(15, sPosy, 0);
    glutSolidSphere(5, 10, 10);
    glPopMatrix();
}

void numBoxes() {
    for (int i = 0; i < 6; i++) {
        glColor3f(0.0, 0.8, 0.8);
        
        glPushMatrix();
        glTranslatef(numbersX[i], 30, 0);
        glutSolidCube(15);
        glColor3f(0.0, 0.0, 0.0);
        char str[3];
        sprintf(str, "%d;", number[i]);
        drawBitmapText(str, -1, -1, 0);
        glPopMatrix();
    }
}

void opBoxes() {
    for (int i = 0; i < 4; i++) {
        glColor3f(1.0, 0.0, 0.2);
        
        glPushMatrix();
        glTranslatef(opX[i], 70, 0);
        glutSolidCube(15);
        glColor3f(1.0, 1.0, 1.0);
        char str[3];
        sprintf(str, "%c;", op[i]);
        drawBitmapText(str, -2, -2, 0);
        glPopMatrix();
    }
}

void startText() {
    if (won == 1) {
        char str[150];
        sprintf(str, "Congratulations, YOU WON!!!! Press 's' to start again...;");
        glColor3f(1.0, 1.0, 1.0);
        drawBitmapText(str, 80.0, 50.0, 0.0);
        return;
    }
    
    char str[150];
    sprintf(str, "Start with 's' (number jump: 'c' | op jump: 'd').;");
    glColor3f(1.0, 1.0, 1.0);
    drawBitmapText(str, 80.0, 50.0, 0.0);
}

void statusText() {
    glColor3f(1.0, 1.0, 1.0);
    drawBitmapText(currentStatus, 110.0, 130.0, 0.0);
}

void scoreText() {
    glColor3f(1.0, 1.0, 1.0);
    char str[5];
    sprintf(str, "SCORE: %d;", score);
    
    drawBitmapText(str, 270.0, 130.0, 0.0);
}

void targetValueText() {
    // Number 1
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3i(130, -15, 0);
    glVertex3i(190, -15, 0);
    glVertex3i(190, -30, 0);
    glVertex3i(130, -30, 0);
    glEnd();
    
    glColor3f(0.0, 0.8, 0.8);
    if (n1 != -1) {
        char nStr[3];
        sprintf(nStr, "%d;", n1);
        drawBitmapText(nStr, 140.0, -25.0, 0.0);
    }
    
    glColor3f(1.0, 0.0, 0.2);
    char opStr[2];
    sprintf(opStr, "%c;", currentOp);
    drawBitmapText(opStr, 150.0, -25.0, 0.0);
    
    glColor3f(0.0, 0.8, 0.8);
    if (n2 != -1) {
        char nStr[3];
        sprintf(nStr, "%d;", n2);
        drawBitmapText(nStr, 160.0, -25.0, 0.0);
    }
    
    glColor3f(1.0, 1.0, 1.0);
    char str[50];
    sprintf(str, "= %d;", currentTarget);
    
    drawBitmapText(str, 170.0, -25.0, 0.0);
}

void lightEffects() {
    // Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);
    
    glMaterialfv(GL_FRONT,GL_SPECULAR, reflex);
    glMateriali(GL_FRONT,GL_SHININESS, objReflex);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient1);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difuse1);
    glLightfv(GL_LIGHT0, GL_SPECULAR, especular1);
    glLightfv(GL_LIGHT0, GL_POSITION, position1);
    
    glLightModelfv(GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE, focal2);
    
    glLightfv(GL_LIGHT1, GL_SPECULAR, focal2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, difuse2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, especular2);
    glLightfv(GL_LIGHT1, GL_POSITION, position2);
    
    // Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    // Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (started == 0) {
        startText();
        glutSwapBuffers();
        return;
    }
    
    //Estrutura
    background();
//    floor();
    
    //Textos
    statusText();
    targetValueText();
    scoreText();
    
    // Personagens
    swingy();
    bouncy();
    
    // Box
    numBoxes();
    opBoxes();
    
    lightEffects();
    
    glutSwapBuffers();
}

void backgroundRoll(int value) {
    if (started == 0) {
        return;
    }
    
    backgroundX1 = backgroundX1 - 7;
    backgroundX2 = backgroundX2 - 7;
    
    if (backgroundX1 + BG_SIZE <= -50) {
        backgroundX1 = backgroundX2 + BG_SIZE;
    }
    
    if (backgroundX2 + BG_SIZE <= -50) {
        backgroundX2 = backgroundX1 + BG_SIZE;
    }
    
    display();
    
    glutPostRedisplay();
    glutTimerFunc(NUMBERGAP, backgroundRoll, value);
}

void numberBoxesWalk(int value) {
    if (started == 0) {
        return;
    }
    
    for (int i = 0; i < 6; i++) {
        numbersX[i] = numbersX[i] - 7;
        if (numbersX[i] <= 0) {
            int r = rand();
            numbersX[i] = 300;
            number[i] = r%10;
        }
    }
    
    display();
    
    glutPostRedisplay();
    glutTimerFunc(NUMBERGAP, numberBoxesWalk, value);
}

void opBoxesWalk(int value) {
    if (started == 0) {
        return;
    }
    
    for (int i = 0; i < 4; i++) {
        opX[i] = opX[i] - 7;
        if (opX[i] <= 0) {
            opX[i] = 300;
        }
    }
    
    display();
    
    glutPostRedisplay();
    glutTimerFunc(OPGAP, opBoxesWalk, value);
}

void onBouncyJumpTimer(int value) {
    if (bPosy <= 0) {
        return;
    }
    
    bPosy = value - 3;
    
    display();
    
    glutPostRedisplay();
    glutTimerFunc(GAP, onBouncyJumpTimer, bPosy);
}

void onSwingyJumpTimer(int value) {
    if (sPosy >= 100) {
        return;
    }
    
    sPosy = value + 3;
    
    display();
    
    glutPostRedisplay();
    glutTimerFunc(GAP, onSwingyJumpTimer, sPosy);
}

int targetIsPossible() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i * j == currentTarget
                || i + j == currentTarget
                || i - j == currentTarget) {
                return 1;
            }
            if (j != 0 && i / j == currentTarget) {
                return 1;
            }
        }
    }
    
    return 0;
}

void reset() {
    
    do {
        currentTarget = rand()%81;
    } while (targetIsPossible() != 1);
    
    currentOp = ' ';
    n1 = -1;
    n2 = -1;
    
    display();
}

void checkResult() {
    int res = 0;
    switch (currentOp) {
        case '+':
            res = n1 + n2;
            break;
        case '-':
            res = n1 - n2;
            break;
        case '*':
            res = n1 * n2;
            break;
        case '/':
            res = n1 / n2;
            break;
        default:
            break;
    }
    
    if (res != currentTarget) {
        sprintf(currentStatus, "Wrong answer (%d != %d);", res, currentTarget);
        reset();
        return;
    }
    
    char str[50];
    sprintf(currentStatus, "RIGHT (%d %c %d = %d);", n1, currentOp, n2, currentTarget);
    score = score + 1;
    
    if (score == SCORE_TO_WIN) {
        started = 0;
        score = 0;
        won = 1;
        sprintf(currentStatus, ";");
    }
    
    reset();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLfloat) w / (GLfloat) h, 1.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Câmera
    float posx = 150.0f, posy = 50.0f, posz = 300.0f, focx = 150.0f, focy = 50.0f, focz = 0.0f, disx = 0.0f, disy = 51.0f, disz = 0.0f;
    gluLookAt(posx, posy, posz, focx, focy, focz, disx, disy, disz);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'c': // Bouncy jump
            if (bPosy > 0){
                break;
            }
            
            bPosy = 25;
            
            for (int i = 0; i < 6; i++) {
                if (numbersX[i] < 30 && numbersX[i] > 0) {
                    if (n1 == -1 || currentOp == ' ') {
                        n1 = number[i];
                    } else {
                        n2 = number[i];
                    }
                    break;
                }
            }
            
            if (n2 != -1) {
                checkResult();
                n1 = -1;
                n2 = -1;
            }
            
            glutTimerFunc(GAP, onBouncyJumpTimer, bPosy);
            glutDisplayFunc(display);
            break;
        case 'd': // Swingy jump
            if (sPosy < 100){
                break;
            }
            
            sPosy = 75;
            
            for (int i = 0; i < 4; i++) {
                if (opX[i] < 30 && opX[i] > 00) {
                    if (n1 != -1) {
                        currentOp = op[i];
                    }
                    break;
                }
            }
            
            glutTimerFunc(GAP, onSwingyJumpTimer, sPosy);
            glutDisplayFunc(display);
            break;
        case 's': // Start
            if (started == 1) break;
            started = 1;
            won = 0;
            glutTimerFunc(OPGAP, opBoxesWalk, 0);
            glutTimerFunc(NUMBERGAP, numberBoxesWalk, 0);
            glutTimerFunc(NUMBERGAP, backgroundRoll, 0);
            glutDisplayFunc(display);
            break;
        default:
            break;
    }
}

int main(int argc, char * * argv) {
    glutInit( & argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Projeto - Swingy & Bouncy");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
