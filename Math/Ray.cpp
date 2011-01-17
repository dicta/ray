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
