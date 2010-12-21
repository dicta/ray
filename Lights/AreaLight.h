/*
 *  AreaLight.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _AREA_LIGHT_H_
#define _AREA_LIGHT_H_

#include "Light.h"
#include "Math/Point3D.h"
#include "Math/Vector3D.h"

class LightObject;

class AreaLight : public Light {
   
public:
   AreaLight();
   virtual ~AreaLight();
   
   virtual Vector3D getLightDirection(const ShadeRecord& sr);
   virtual void setHash(Hash* hash);
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);
   
   virtual Color L(const ShadeRecord& sr);
   virtual float G(const ShadeRecord& sr);
   virtual float pdf(const ShadeRecord& sr);
   
private:
   LightObject* object;
   Material* material;
   Point3D samplePoint;
   Vector3D lightNormal;
   Vector3D wi;
};

#endif
