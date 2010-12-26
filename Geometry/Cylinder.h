/*
 *  Cylinder.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "GeometryObject.h"

class Cylinder : public GeometryObject {
   
public:
   Cylinder();
   
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
private:
   float radius;
   float minY;
   float maxY;
};

#endif
