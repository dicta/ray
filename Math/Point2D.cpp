/*
 *  Point2D.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Point2D.h"

Point2D::Point2D() : x(0), y(0) {
}

Point2D::Point2D(float x, float y) : x(x), y(y) {
}

Point2D Point2D::operator*(const float a) const {
   Point2D p(x * a, y * a);
   return p;
}
