/*
 *  Point2D.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _POINT2D_H_
#define _POINT2D_H_

class Point2D {

public:
   Point2D();
   Point2D(float x, float y);
   
   Point2D operator*(const float a) const;
   
   float x, y;
};

inline Point2D Point2D::operator*(const float a) const {
   return Point2D(x * a, y * a);
}

#endif
