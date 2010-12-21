/*
 *  Sphere.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "GeometryObject.h"
#include "Math/Point3D.h"

class Sphere : public GeometryObject {
   
public:
   Sphere();
   Sphere(const Point3D& c, double r = 1.0);

   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;

   void setCenter(const Point3D& p) { center = p; }
   void setRadius(double r) { radius = r; }
   
   virtual void setHash(Hash* hash);
   
private:
   Point3D center;
   double radius;
};

#endif
