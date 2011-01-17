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

class Ray {
   
public:
   Ray();
   Ray(const Point3D& o, const Vector3D& d);
   Ray(const Ray& ray);

   Point3D origin;
   Vector3D direction;
};

#endif
