/*
 *  Rectangle.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "LightObject.h"
#include "Math/Point3D.h"
#include "Math/Vector3D.h"

class Sampler;

class Rectangle : public LightObject {
   
public:
   Rectangle();
   ~Rectangle();
   
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   virtual void setHash(Hash* hash);
   
   virtual Point3D sample() const;
   virtual Vector3D getNormal(const Point3D& point) const;
   virtual double pdf(const ShadeRecord& sr) const;
   
private:
   Point3D origin;
   Vector3D a;
   Vector3D b;
   Vector3D normal;
   Sampler* sampler;
   
   double lengthASquared;
   double lengthBSquared;
   double inverseArea;
};

#endif
