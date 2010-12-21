/*
 *  AmbientOccluder.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _AMBIENT_OCCLUDER_H_
#define _AMBIENT_OCCLUDER_H_

#include "Ambient.h"

class Sampler;

class AmbientOccluder : public Ambient {
   
public:
   AmbientOccluder();
   virtual ~AmbientOccluder();

   virtual Vector3D getLightDirection(const Point3D& hitPoint);
   virtual void setHash(Hash* hash);
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);
   
   virtual Color L(const ShadeRecord& sr);
   
private:
   Color minColor;
   Sampler* sampler;
   Vector3D u, v, w;
};

#endif
