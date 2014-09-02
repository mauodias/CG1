//
//  ShapeDrawer.cpp
//  OpenGL Tutorial
//
//  Created by Maurício Dias on 2014-09-01.
//  Copyright (c) 2014 Maurício Dias. All rights reserved.
//

#include "ShapeDrawer.h"

void ShapeDrawer::drawRectangle(RECTANGLE rectangle, COLOR fillColor, COLOR borderColor, RectangleType type) {
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

void ShapeDrawer::drawCircle(CIRCLE circle, COLOR color)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(color.r, color.g, color.b);
    glVertex2f(circle.x, circle.y);
    int numSegments = 100;
    GLfloat angle;
    for (int i = 0; i < numSegments; i++) {
        angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
        glVertex2f(cos(angle) * circle.radius, sin(angle) * circle.radius);
    }
    glEnd();
}

void ShapeDrawer::buildWalls(WALL wall)
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
        ShapeDrawer::drawRectangle(rect, red, white, RectangleType::BOTH);
        
        rect.left = rect.right;
        rect.right += 0.2;
    }
}