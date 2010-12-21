/*
 *  Tracer.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TRACER_H_
#define _TRACER_H_

#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Ray.h"

class Tracer {
   
public:
   virtual Color traceRay(const Ray& ray) = 0;
   ShadeRecord hitObjects(const Ray& ray);
};

#endif
