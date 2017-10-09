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
#include <stdio.h>
#include <stdlib.h>

//GAP do contador entre carregamentos
#define NUMBERGAP 100
#define OPGAP 100
#define GAP 50

static int sPosy = 100, bPosy = 0;
static int numbersX[6] = {50, 100, 150, 200, 250, 300};
static int number[6] = {1, 2, 3, 4, 5, 6};
static int opX[4] = {75, 150, 225, 300};
static char op[4] = {'+', '-', '*', '/'};

static int n1 = -1, n2 = -1;
static char currentOp = ' ';
static int currentTarget = 4;
static char * currentStatus = "Para começar aperte 's' (pulos com 'x' e 'c');";

static int started = 0;

static int score = 0;

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_FLAT);
}

void drawBitmapText(char * string, float x, float y, float z) {
    char * c;
    
    glRasterPos3f(x, y, z);
    
    for (c = string; * c != ';'; c++) {
        
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, * c);
    }
}

//void axis() {
//
//    // x
//    glPushMatrix();
//    glBegin(GL_LINES);
//    glColor3f(1, 0, 0);
//    glVertex3i(0, 0, 0);
//    glVertex3i(100, 0, 0);
//    glEnd();
//    glPopMatrix();
//
//    // y
//    glPushMatrix();
//    glBegin(GL_LINES);
//    glColor3f(0, 1, 0);
//    glVertex3i(0, 0, 0);
//    glVertex3i(0, 100, 0);
//    glEnd();
//    glPopMatrix();
//
//    // z
//    glPushMatrix();
//    glBegin(GL_LINES);
//    glColor3f(0, 0, 1);
//    glVertex3i(0, 0, 0);
//    glVertex3i(0, 0, 100);
//    glEnd();
//    glPopMatrix();
//}

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
    glColor3f(0.0, 0.8, 0.4);

    glPushMatrix();
    glTranslatef(15, bPosy, 0);
    glutSolidSphere(5, 10, 10);
    glPopMatrix();
}

void swingy() {
    glColor3f(0.4, 0.0, 1.0);
    
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
        glutSolidCube(10);
        glColor3f(0.0, 0.0, 0.0);
        char str[3];
        sprintf(str, "%d;", number[i]);
        drawBitmapText(str, 0, 0, 0);
        glPopMatrix();
    }
}

void opBoxes() {
    for (int i = 0; i < 4; i++) {
        glColor3f(1.0, 0.0, 0.2);
        
        glPushMatrix();
        glTranslatef(opX[i], 70, 0);
        glutSolidCube(10);
        glColor3f(0.0, 0.0, 0.0);
        char str[3];
        sprintf(str, "%c;", op[i]);
        drawBitmapText(str, 0, 0, 0);
        glPopMatrix();
    }
}

void statusText() {
    glColor3f(0.0, 0.0, 0.0);
    drawBitmapText(currentStatus, 120.0, 120.0, 0.0);
}

void scoreText() {
    glColor3f(0.0, 0.0, 0.0);
    char str[5];
    sprintf(str, "Pontuacao %d;", score);
    
    drawBitmapText(str, 275.0, 120.0, 0.0);
}

void targetValueText() {
    glColor3f(0.0, 0.0, 0.0);
    char str[50];
    sprintf(str, "%d %c %d = %d;", n1, currentOp, n2, currentTarget);
    
    drawBitmapText(str, 300.0, 0.0, 0.0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    //Estrutura
    floor();
    
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
    
    glutSwapBuffers();
}

void numberBoxesWalk(int value) {
    for (int i = 0; i < 6; i++) {
        numbersX[i] = numbersX[i] - 4;
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
    for (int i = 0; i < 4; i++) {
        opX[i] = opX[i] - 4;
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

//void fimDeJogoD() {
//
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    //Estrutura
//    eixos();
//    pista();
//
//    //Textos
//    drawBitmapText("COMO? ISSO EH IMPOSSIVEL!!!!!!;", 1.75, 0.95, 0.0);
//    drawBitmapText("VOCE VENCEU;", 1.65, 0.95, 0.0);
//    recomecar = 1;
//
//    //Torres (Ordem contrária para fazer sobreposição)
//    torreEsquerda(0);
//    torreMeioEsquerdaB(0);
//    torreMeioEsquerdaA(0);
//    torreMeioB(3);
//    torreMeioA(0);
//    torreMeioDireitaB(3);
//    torreMeioDireitaA(3);
//    torreDireita(3);
//
//    //Chefões das Torres (Ordem contrária para fazer sobreposição)
//    chefeTorreEsquerda(0);
//    chefeTorreMeioEsquerdaB(0);
//    chefeTorreMeioEsquerdaA(0);
//    chefeTorreMeioB(0);
//    chefeTorreMeioA(3);
//    chefeTorreMeioDireitaB(3);
//    chefeTorreMeioDireitaA(3);
//    chefeTorreDireita(3);
//
//    glutSwapBuffers();
//
//}

void reset() {
    currentTarget = rand()%81;
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
        currentStatus = "EROOOOOOU;";
        reset();
        return;
    }
    
    currentStatus = "BOOOOOOUAAAA!!;";
    score = score + 1;
    reset();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, (GLfloat) w / (GLfloat) h, 1.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Câmera
    float posx = 150.0f, posy = 50.0f, posz = 100.0f, focx = 150.0f, focy = 50.0f, focz = 0.0f, disx = 0.0f, disy = 51.0f, disz = 0.0f;
    gluLookAt(posx, posy, posz, focx, focy, focz, disx, disy, disz);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'x': // Bouncy jump
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
        case 'c': // Swingy jump
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
            glutTimerFunc(OPGAP, opBoxesWalk, 0);
            glutTimerFunc(NUMBERGAP, numberBoxesWalk, 0);
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
