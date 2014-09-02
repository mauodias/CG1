//
//  ShapeDrawer.h
//  OpenGL Tutorial
//
//  Created by Maurício Dias on 2014-09-01.
//  Copyright (c) 2014 Maurício Dias. All rights reserved.
//

#ifndef __OpenGL_Tutorial__ShapeDrawer__
#define __OpenGL_Tutorial__ShapeDrawer__

#include <iostream>
#include <math.h>
#include "ShapeDefinitions.h"
#include "Physics.h"

class ShapeDrawer {
public:
    static void drawRectangle(RECTANGLE rectangle, COLOR fillColor, COLOR borderColor, RectangleType type);
    static void drawCircle(CIRCLE circle, COLOR color);
    static void buildWalls(WALL wall);
    
};

#endif /* defined(__OpenGL_Tutorial__ShapeDrawer__) */
