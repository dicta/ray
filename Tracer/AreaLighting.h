/*
 *  AreaLighting.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _AREA_LIGHTING_H_
#define _AREA_LIGHTING_H_

#include "Tracer.h"
#include "Math/Ray.h"

class AreaLighting : public Tracer {
   
public:
   AreaLighting();
   virtual ~AreaLighting() {}
   
   Color traceRay(const Ray& ray);
};

#endif
