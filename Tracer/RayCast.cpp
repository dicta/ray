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
#include "Geometry/GeometryManager.h"

RayCast::RayCast() : Tracer() {
}

Color RayCast::traceRay(const Ray& ray, const int depth) {
   if(depth > maxDepth) {
      return BLACK;
   }

   double t = 0.0;
   ShadeRecord sr;

   if(GeometryManager::instance().getGrid().hit(ray, t, sr)) {
      sr.tracer = this;
      sr.depth = depth;
      Color c = sr.material->shade(sr, ray);

      if(c.getAlphaF() < 1.0) {
         Ray newRay(sr.localHitPoint, ray.direction);
         Color newColor = traceRay(newRay, depth);
         return c * c.alpha + newColor * (1.0 - c.alpha);
      }
      return c;
   }
   else {
      if(texture != NULL) {
         sr.localHitPoint.set(ray.direction.x, ray.direction.y, ray.direction.z);
         return texture->getColor(sr);
      }
      return bgColor;
   }
}
