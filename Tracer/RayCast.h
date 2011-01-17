/*
 *  RayCast.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _RAY_CAST_H_
#define _RAY_CAST_H_

#include "Tracer.h"

class RayCast : public Tracer {
   
public:
   RayCast();
   virtual ~RayCast() {}
   
   Color traceRay(const Ray& ray, const int depth);
};

#endif
