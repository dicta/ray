/*
 *  PositionLight.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _POSITION_H_
#define _POSITION_H_

#include "Light.h"

class PositionLight : public Light {

public:
   PositionLight();
   PositionLight(const Point3D& loc);
   virtual ~PositionLight() {}

   virtual Vector3D getLightDirection(ShadeRecord& sr);
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);
   virtual void setHash(Hash* hash);
   virtual Color L(const ShadeRecord& sr);
   
private:
   float ls;
   Color color;
   Point3D location;
};

#endif
