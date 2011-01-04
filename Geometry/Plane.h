/*
 *  Plane.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PLANE_H_
#define _PLANE_H_

#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "GeometryObject.h"

class Hash;

class Plane : public GeometryObject {
   
public:
   Plane();

   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   virtual void setHash(Hash* hash);

private:
   Point3D point;
   Vector3D normal;
};

#endif
