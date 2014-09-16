#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
//#include "stdafx.h"
using namespace std;

//
//  main.cpp
//  OpenGL Tutorial
//
//  Created by Maurício Dias on 2014-08-20.
//  Copyright (c) 2014 Maurício Dias. All rights reserved.
//
#include "ShapeDrawer.h"
#include <mmsystem.h>


// Global variables
int up, down, Left, Right = 0;

char title[] = "Full-Screen & Windowed Mode";  // Windowed mode's title
char pressedKey [1];
int windowWidth  = 320;     // Windowed mode's width
int windowHeight = 640;     // Windowed mode's height
int windowPosX   = 50;      // Windowed mode's top-left corner x
int windowPosY   = 50;      // Windowed mode's top-left corner y
int lvl = 0;
int score= 0;
int difficulty = 10;
bool transportToNewLevel = false;
bool canJump = true;
bool needInitialDraw = true;
bool refreshTime = false;
bool started, stopped = false;
struct timeval firstTime, currentTime;
WALL walls[300];
int musicTempo[300];
int lastPosAvaiable, prevLastPos = 0;    // Last position available at
CIRCLE circle;
COLOR blue = {.r = 0.0f, .g = 0.0f, .b = 1.0f};
GLfloat ballRadius = 0.05f;   // Radius of the bouncing ball
GLfloat ballX = -0.9f;        // Ball's center (x, y) position
GLfloat ballY = 1.4f;
GLfloat ballXMax, ballXMin, ballYMax, ballYMin; // Ball's center (x, y) bounds
GLfloat xSpeed = 0.1f;      // Ball's speed in x and y directions
GLfloat ySpeed = 0.007f;
int refreshMillis =15;      // Refresh period in milliseconds

// Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

/* Initialize OpenGLstring Graphics */
void initGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
}

void drawBitmapText(char *string,float x,float y,float z)
{
	char *c;
	glRasterPos3f(x, y,z);

	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}


void detectCollision(){
    float ballTop, ballBottom, ballLeft, ballRight;

    ballTop = ballY + ballRadius;
    ballBottom = ballY - ballRadius;
    ballLeft = ballX - ballRadius;
    ballRight = ballX + ballRadius;

    // Check if the ball exceeds the edges
    if (ballX > ballXMax) {
        ballX = ballXMax;
        xSpeed = -xSpeed;
    } else if (ballX < ballXMin) {
        ballX = ballXMin;
        //xSpeed = -xSpeed;
    }
    if (ballY > ballYMax) {
        if(score > 300) stopped = true;
        //ballY = ballYMax;
        //ySpeed = -ySpeed;
    } else if (ballY < ballYMin) {
        transportToNewLevel = true;
        ballY = ballYMin;
        ySpeed = 0;
        xSpeed *= 0.9f;
    }


    for (int i = 0; i < lastPosAvaiable; i++) {
        bool foundCollision = false;
        float wallTop = (walls[i].level+1)*0.2f - 2.0f;
        float wallBottom = wallTop - 0.2f;
        float wallHoleLeft = (walls[i].holePosition)*0.2f - 1.0f;
        float wallHoleRight = wallHoleLeft + walls[i].holeSize*0.2f;
        canJump = false;
        if (transportToNewLevel){
            wallTop = (walls[lastPosAvaiable-1].level+1)*0.2f - 2.0f;
            ballY = wallTop+ballRadius;
            ySpeed = 0;
            xSpeed *= 0.9f;
            foundCollision = true;
            transportToNewLevel = false;
        }
        else if (ballBottom < wallTop) {
            if (ballTop > wallBottom) {
                canJump = true;
                if ((ballLeft < wallHoleLeft || ballRight > wallHoleRight)) {
                    ballY = wallTop+ballRadius;
                    ySpeed = 0;
                    xSpeed *= 0.9f;
                    foundCollision = true;
                }
            }
        }
        else if (ballTop > wallBottom) {
            if (ballBottom < wallTop) {
                if ((ballLeft < wallHoleLeft || ballRight > wallHoleRight)) {
                    ballY = wallBottom-ballRadius;
                    ySpeed = -ySpeed;
                    foundCollision = true;
                }
            }
        }
        if (ballLeft < wallHoleLeft) {
            if (ballRight > wallHoleRight) {
                if (ballTop < wallBottom || ballBottom > wallTop) {
                    ballX = wallHoleLeft - ballRadius;
                    xSpeed = -xSpeed;
                    foundCollision = true;
                }
            }
        }
        else if (ballRight > wallHoleRight){
            if (ballLeft < wallHoleLeft) {
                if (ballTop < wallBottom || ballBottom > wallTop) {
                    ballX = wallHoleRight + ballRadius;
                    xSpeed = -xSpeed;
                    foundCollision = true;
                }
            }
        }

        if (foundCollision) {
            break;
        }
    }
}

void moveUp(){
    for (int x = 0; x < lastPosAvaiable; x++)
        walls[x].level++;
}

void generateRandomWall(int listPos){
    int holeSize = 2 + (rand() % (int)(3 - 2 + 1));
    int holePosition = 0 +(rand () % (int) ((10-holeSize) - 0 +1));
    walls[listPos].holePosition = holePosition;
    walls[listPos].holeSize = holeSize;
    lastPosAvaiable++;
}

void generateInitialLevels(){
    generateRandomWall(0);
    generateRandomWall(1);
    generateRandomWall(2);
    generateRandomWall(3);
    generateRandomWall(4);
    walls[0].level = 0;
    walls[1].level = 4;
    walls[2].level = 8;
    walls[3].level = 12;
    walls[4].level = 16;
}

/*  Function will calculate the difference between two timestamps in milliseconds */
int diff_ms(timeval t1, timeval t2)
{
    return (((t1.tv_sec - t2.tv_sec) * 1000000) +
            (t1.tv_usec - t2.tv_usec))/1000;
}
void loadMusicTempo(){
    int x=0;
    string line;
    ifstream myfile ("musicTempo.txt");
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            musicTempo[x] = atoi(line.c_str());
            x++;
        }
        myfile.close();
    }
}
void startLevel(){
    loadMusicTempo();
    PlaySound("song.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
    refreshTime = true;
    started = true;
    stopped = false;
    score = 0;
    prevLastPos = lastPosAvaiable;
}

/* Callback handler for window re-paint event */
void display() {
    int multiplier = (lastPosAvaiable - prevLastPos)-1;

    if(!stopped) score = multiplier * 100;

    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
    glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
        // Draws only initial levels once
    if (needInitialDraw){
        generateInitialLevels();
        needInitialDraw=false;
    }

    // Draws every designed wall
    for (int x = 0; x < lastPosAvaiable; x++){
        ShapeDrawer::buildWalls(walls[x]);
    }

    if(!started){
        glColor3f(0,1,0);
        drawBitmapText("Select a difficulty to start:",-0.7f,1,0);
        glColor3f(0,1,0);
        drawBitmapText("'F1' for Easy",-0.4f,0.85f,0);
        glColor3f(0,1,0);
        drawBitmapText("'F2' for Normal",-0.4f,0.7f,0);
        glColor3f(0,1,0);
        drawBitmapText("'F3' for Hard",-0.4f,0.55f,0);
    }else {
        string Result;          // string which will contain the result
        std::ostringstream convert;   // stream used for the conversion
        convert << score;      // insert the textual representation of 'Number' in the characters in the stream

        Result = "Score: " + convert.str(); // set 'Result' to the contents of the stream
        std::string str;
        char * writable = new char[Result.size() + 1];
        std::copy(Result.begin(), Result.end(), writable);
        writable[Result.size()] = '\0';
        glColor3f(0,1,0);
        drawBitmapText(writable,-0.2f,1,0);
        delete[] writable;
        if(stopped){
            glColor3f(0,1,0);
            drawBitmapText("Game over my friend :(",-0.5f,0.85f,0);
            glColor3f(0,1,0);
            drawBitmapText("Select a difficulty to restart:",-0.7f,0.7f,0);
            glColor3f(0,1,0);
            drawBitmapText("'F1' for Easy",-0.4f,0.55f,0);
            glColor3f(0,1,0);
            drawBitmapText("'F2' for Normal",-0.4f,0.4f,0);
            glColor3f(0,1,0);
            drawBitmapText("'F3' for Hard",-0.4f,0.25f,0);
        }
    }


    // Will detect if its needed to draw another lower level
    bool existsAtZero, existsAtOne, existsAtTwo, existsAtThree = false;
    for (int x = 0; x < lastPosAvaiable; x++){
        if (walls[x].level==0) existsAtZero = true;
        if (walls[x].level==1) existsAtOne = true;
        if (walls[x].level==2) existsAtTwo = true;
        if (walls[x].level==3) existsAtThree = true;
        if (x == lastPosAvaiable - 1 and !existsAtZero and !existsAtOne and !existsAtTwo and !existsAtThree){
            generateRandomWall(lastPosAvaiable);
            walls[lastPosAvaiable].level = 0;
        }
    }

    if(refreshTime){
        gettimeofday (&firstTime, NULL);
        refreshTime = false;
    }

    // If last time the floor moved is greater than the fall speed
    // means it needs to go up again
    gettimeofday (&currentTime, NULL);
    int diff = diff_ms(currentTime, firstTime);
    for (int x = 0; x < 300; x++){
        if(started && ((diff - difficulty) < musicTempo[x] && musicTempo[x] < (diff + difficulty))){
            moveUp();
        }
    }


    glTranslatef(ballX, ballY, 0.0f);  // Translate to (xPos, yPos)
    // Use triangular segments to form a circle

    circle.x = circle.y = 0;
    circle.radius = ballRadius;
    ShapeDrawer::drawCircle(circle, blue);

    /*glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f, 0.0f, 1.0f);  // Blue
    glVertex2f(0.0f, 0.0f);       // Center of circle
    int numSegments = 100;
    GLfloat angle;
    for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
        angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
        glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
    }
    glEnd();*/
    glLoadIdentity();              // Reset model-view matrix

    glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)

    // Animation Control - compute the location for the next refresh
    ySpeed -=  GRAVITY/1000.0f/refreshMillis*METER;
    ballX += xSpeed;
    ballY += ySpeed;

    // Check if the ball exceeds the edges
    /*if (ballX > ballXMax) {
        ballX = ballXMax;
        xSpeed = -xSpeed;
    } else if (ballX < ballXMin) {
        ballX = ballXMin;
        xSpeed = -xSpeed;
    }
    if (ballY > ballYMax) {
        ballY = ballYMax;
        ySpeed = -ySpeed;
    } else if (ballY < ballYMin) {
        ballY = ballYMin;
        ySpeed *= -0.85;
        xSpeed *= 0.9;
    }*/


    detectCollision();
}

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    if (width >= height) {
        clipAreaXLeft   = -1.0 * aspect;
        clipAreaXRight  = 1.0 * aspect;
        clipAreaYBottom = -1.0;
        clipAreaYTop    = 1.0;
    } else {
        clipAreaXLeft   = -1.0;
        clipAreaXRight  = 1.0;
        clipAreaYBottom = -1.0 / aspect;
        clipAreaYTop    = 1.0 / aspect;
    }
    gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
    ballXMin = clipAreaXLeft + ballRadius;
    ballXMax = clipAreaXRight - ballRadius;
    ballYMin = clipAreaYBottom + ballRadius;
    ballYMax = clipAreaYTop - ballRadius;
}

/* Called back when the timer expired */
void Timer(int value) {
    glutPostRedisplay();    // Post a paint request to activate display()
    glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
}

/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:     // ESC key
            exit(0);
            break;
    }
}

/* Callback handler for special-key event */
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            difficulty=10;
            startLevel();
            break;
        case GLUT_KEY_F2:
            difficulty=30;
            startLevel();
            break;
        case GLUT_KEY_F3:
            difficulty=50;
            startLevel();
            break;
        case GLUT_KEY_RIGHT:    // Right: increase x speed
            if(!stopped){
                xSpeed = 0.03f;
                Right = 10;
            }
            break;
        case GLUT_KEY_LEFT:     // Left: decrease x speed
            if(!stopped){
                xSpeed = -0.03f;
                Left = 10;
            }
            break;
        case GLUT_KEY_UP:       // Up: increase y speed
            if (canJump){
                ySpeed += /*200*METER/1000/15*/0.08f;
            }
            break;
        case GLUT_KEY_DOWN:     // Down: decrease y speed
            ySpeed -= 0.06f;
            down = 10;
            break;
        case GLUT_KEY_PAGE_UP:  // Page-Up: increase ball's radius
            ballRadius *= 1.05f;
            ballXMin = clipAreaXLeft + ballRadius;
            ballXMax = clipAreaXRight - ballRadius;
            ballYMin = clipAreaYBottom + ballRadius;
            ballYMax = clipAreaYTop - ballRadius;
            break;
        case GLUT_KEY_PAGE_DOWN: // Page-Down: decrease ball's radius
            ballRadius *= 0.95f;
            ballXMin = clipAreaXLeft + ballRadius;
            ballXMax = clipAreaXRight - ballRadius;
            ballYMin = clipAreaYBottom + ballRadius;
            ballYMax = clipAreaYTop - ballRadius;
            break;
    }
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
    glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
    glutCreateWindow(title);      // Create window with given title
    glutDisplayFunc(display);     // Register callback handler for window re-paint
    glutReshapeFunc(reshape);     // Register callback handler for window re-shape
    glutTimerFunc(0, Timer, 0);   // First timer call immediately
    glutSpecialFunc(specialKeys); // Register callback handler for special-key event
    glutKeyboardFunc(keyboard);   // Register callback handler for special-key event
    //glutFullScreen();             // Put into full screen
    initGL();                     // Our own OpenGL initialization
    glutMainLoop();               // Enter event-processing loop
    return 0;
}
