#include <stdlib.h>
#include <GL/glut.h>
//
//  main.cpp
//  OpenGL Tutorial
//
//  Created by Maurício Dias on 2014-08-20.
//  Copyright (c) 2014 Maurício Dias. All rights reserved.
//

#include "ShapeDrawer.h"


// Global variables
int up, down, left, right = 0;

char title[] = "Full-Screen & Windowed Mode";  // Windowed mode's title
char pressedKey [1];
int windowWidth  = 320;     // Windowed mode's width
int windowHeight = 640;     // Windowed mode's height
int windowPosX   = 50;      // Windowed mode's top-left corner x
int windowPosY   = 50;      // Windowed mode's top-left corner y
int lvl = 0;
WALL walls[5];
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

bool fullScreenMode = false; // Full-screen or windowed mode?

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
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
        xSpeed = -xSpeed;
    }
    if (ballY > ballYMax) {
        exit(0);
        //ballY = ballYMax;
        //ySpeed = -ySpeed;
    } else if (ballY < ballYMin) {
        ballY = ballYMin;
        ySpeed = 0;
        xSpeed *= 0.9f;
    }


    for (int i = 0; i < 5; i++) {
        bool foundCollision = false;
        float wallTop = (walls[i].level+1)*0.2f - 2.0f;
        float wallBottom = wallTop - 0.2f;
        float wallHoleLeft = (walls[i].holePosition)*0.2f - 1.0f;
        float wallHoleRight = wallHoleLeft + walls[i].holeSize*0.2f;
        if (ballBottom < wallTop) {
            if (ballTop > wallBottom) {
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

/* Callback handler for window re-paint event */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
    glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
    walls[0].level = 0 + lvl;
    walls[0].holePosition = 2;
    walls[0].holeSize = 2;
    walls[1].level = 4 + lvl;
    walls[1].holePosition = 6;
    walls[1].holeSize = 3;
    walls[2].level = 8 + lvl;
    walls[2].holePosition = 4;
    walls[2].holeSize = 3;
    walls[3].level = 12 + lvl;
    walls[3].holePosition = 8;
    walls[3].holeSize = 2;
    walls[4].level = 16 + lvl;
    walls[4].holePosition = 5;
    walls[4].holeSize = 4;

    ShapeDrawer::buildWalls(walls[0]);
    ShapeDrawer::buildWalls(walls[1]);
    ShapeDrawer::buildWalls(walls[2]);
    ShapeDrawer::buildWalls(walls[3]);
    ShapeDrawer::buildWalls(walls[4]);

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
        case GLUT_KEY_HOME: // Home: increase level height
            lvl++;
            break;
        case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
            fullScreenMode = !fullScreenMode;         // Toggle state
            if (fullScreenMode) {                     // Full-screen mode
                windowPosX   = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
                windowPosY   = glutGet(GLUT_WINDOW_Y);
                windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
                windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
                glutFullScreen();                      // Switch into full screen
            } else {                                         // Windowed mode
                glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
                glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
            }
            break;
        case GLUT_KEY_RIGHT:    // Right: increase x speed
            xSpeed = 0.03f;
            right = 10;
            break;
        case GLUT_KEY_LEFT:     // Left: decrease x speed
            xSpeed = -0.03f;
            left = 10;
            break;
        case GLUT_KEY_UP:       // Up: increase y speed
            ySpeed += /*200*METER/1000/15*/0.06f;
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
