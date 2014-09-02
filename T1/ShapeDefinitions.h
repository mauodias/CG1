//
//  ShapeDefinitions.h
//  OpenGL Tutorial
//
//  Created by Maurício Dias on 2014-09-01.
//  Copyright (c) 2014 Maurício Dias. All rights reserved.
//

#ifndef OpenGL_Tutorial_ShapeDefinitions_h
#define OpenGL_Tutorial_ShapeDefinitions_h

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

struct WALL {
    GLint level, holePosition, holeSize;
};

struct RECTANGLE {
    GLfloat left, right, top, bottom;
};

enum class RectangleType {
    BORDER,
    FILL,
    BOTH
};

struct CIRCLE {
    GLfloat x, y, radius;
};

struct COLOR {
    GLfloat r, g, b, a;
};

#endif
