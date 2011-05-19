/*
 *  Ray.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Ray.h"

Ray::Ray() : origin(Point3D(0, 0, 0)), direction(Vector3D(0, 0, -1)) {
}

Ray::Ray(const Point3D& o, const Vector3D& d) : origin(o), direction(d) {
}

Ray::Ray(const Ray& ray) : origin(ray.origin), direction(ray.direction) {
}

Matrix Ray::calculateNormalMatrix() const {
   Point3D n = origin - direction;
   Vector3D nVec(n.x, n.y, n.z);
   Vector3D uVec = Vector3D(0, 1, 0).cross(nVec);
   nVec.normalize();
   uVec.normalize();
   Vector3D vVec = nVec.cross(uVec);

   Matrix matrix(uVec, vVec, nVec);
   matrix.invert();
   return matrix;
}
