/*
 *  AreaLighting.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AreaLighting.h"
#include "Materials/Material.h"

AreaLighting::AreaLighting() : Tracer() {
}

Color AreaLighting::traceRay(const Ray& ray) {
   ShadeRecord sr = hitObjects(ray);
   
   if(sr.hit) {
      return sr.material->areaLightShade(sr, ray);
   }
   else {
      return BLACK;
   }
}
