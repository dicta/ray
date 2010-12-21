/*
 *  MultiObjects.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MULTI_OBJECTS_H_
#define _MULTI_OBJECTS_H_

#include "Tracer.h"
#include "Utility/ShadeRecord.h"

class MultiObjects : public Tracer {

public:
   virtual Color traceRay(const Ray& ray);
   
private:
   ShadeRecord sr;
};

#endif
