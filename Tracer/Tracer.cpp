/*
 *  Tracer.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>
#include "Tracer.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"

ShadeRecord Tracer::hitObjects(const Ray& ray) {
   double t = 0.0;
   ShadeRecord sr;

   if(GeometryManager::instance().getGrid().hit(ray, t, sr)) {
//      sr.hit = true;
      sr.tracer = this;
   }

   return sr;
}
