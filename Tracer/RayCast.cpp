/*
 *  RayCast.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "RayCast.h"
#include "Materials/Material.h"

RayCast::RayCast() : Tracer() {
}

Color RayCast::traceRay(const Ray& ray) {
   ShadeRecord sr = hitObjects(ray);
   
   if(sr.hit) {
      return sr.material->shade(sr, ray);
   }
   else {
      return BLACK;
   }
}
