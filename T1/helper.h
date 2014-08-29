//
//  helper.h
//  OpenGL Tutorial
//
//  Created by Maurício Dias on 2014-08-26.
//  Copyright (c) 2014 Maurício Dias. All rights reserved.
//

#ifndef OpenGL_Tutorial_helper_h
#define OpenGL_Tutorial_helper_h

#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#include <iostream>
#include <stdlib.h>
#include <Math.h>     // Needed for sin, cos
#define PI 3.14159265f

// Real world physics simulation
#define METER 10
#define GRAVITY 9.81f

enum class RectangleType {
    BORDER,
    FILL,
    BOTH
};

struct WALL {
    GLint level, holePosition, holeSize;
};

struct RECTANGLE {
    GLfloat left, right, top, bottom;
};

struct CIRCLE {
    GLfloat x, y, radius;
};

struct COLOR {
    GLfloat r, g, b, a;
};

void drawText(char*string,int x,int y)
{
    char *c;
    glPushMatrix();
    glTranslatef(x, y,0);
    glScalef(0.1,-0.1,1);
    
    for (c=string; *c != '\0'; c++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN , *c);
    }
    glPopMatrix();
    
}

void drawRectangle(RECTANGLE rectangle, COLOR fillColor, COLOR borderColor, RectangleType type)
{
    if (type == RectangleType::FILL || type == RectangleType::BOTH) {
        glBegin(GL_QUADS);
        glColor3f(fillColor.r, fillColor.g, fillColor.b);
        glVertex2f(rectangle.left, rectangle.bottom);
        glVertex2f(rectangle.right, rectangle.bottom);
        glVertex2f(rectangle.right, rectangle.top);
        glVertex2f(rectangle.left, rectangle.top);
        glEnd();
    }
    if (type == RectangleType::BOTH || type == RectangleType::BORDER) {
        glBegin(GL_LINE_LOOP);
        glColor3f(borderColor.r, borderColor.g, borderColor.b);
        glVertex2f(rectangle.left, rectangle.bottom);
        glVertex2f(rectangle.right, rectangle.bottom);
        glVertex2f(rectangle.right, rectangle.top);
        glVertex2f(rectangle.left, rectangle.top);
        glEnd();
    }
    

}

void drawCircle(CIRCLE circle, COLOR color)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(color.r, color.g, color.b);
    glVertex2f(circle.x, circle.y);
    int numSegments = 100;
    GLfloat angle;
    for (int i = 0; i < 100; i++) {
        angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
        glVertex2f(cos(angle) * circle.radius, sin(angle) * circle.radius);
    }
    glEnd();
}

void buildWalls(WALL wall)
{
    RECTANGLE rect;
    COLOR red, white;
    rect.bottom = -2.0f + 0.2f*wall.level;
    rect.top = rect.bottom + 0.2f;
    rect.left = -1.0f;
    rect.right = rect.left + 0.2f;
    red.r = 1.0f;
    white.r = white.g = white.b = 1.0f;
    
    for (int i = 0; i < 10; i++) {
        if(i >= wall.holePosition && i < wall.holePosition+wall.holeSize)
        {
            rect.left = rect.right;
            rect.right += 0.2;
            continue;
        }
        drawRectangle(rect, red, white, RectangleType::BOTH);

        rect.left = rect.right;
        rect.right += 0.2;
    }
    
}


#endif
