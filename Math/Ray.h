/*
 *  Ray.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RAY_H_
#define _RAY_H_

#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "Math/Matrix.h"

class Ray {
   
public:
   Ray();
   Ray(const Point3D& o, const Vector3D& d);
   Ray(const Ray& ray);
   
   Point3D operator()(double t) const { return origin + direction * t; }
   /**
    * Calculates a matrix that can be used to convert a normal from model spsace to eye space.
    */
   Matrix calculateNormalMatrix() const;

   Point3D origin;
   Vector3D direction;
};

#endif
