/*
 *  LightObject.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _LIGHT_OBJECT_H_
#define _LIGHT_OBJECT_H_

#include "GeometryObject.h"
#include "Math/Vector3D.h"

class LightObject : public GeometryObject {

public:
   LightObject() {}
   virtual ~LightObject() {}

   virtual Point3D sample(const Point3D& hitPoint) const = 0;
   virtual Vector3D getNormal(const Point3D& point) const = 0;
   virtual double pdf(const ShadeRecord& sr) const = 0;
};

#endif
