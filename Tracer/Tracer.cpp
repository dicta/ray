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
   double t;
   double tmin = 1.7 * pow(10, 308);
   ShadeRecord sr;
   Vector3D normal;
   Point3D localHitPoint;

   for(GeometryIter it = GeometryManager::instance().begin(); it != GeometryManager::instance().end(); it++) {
      if((*it)->hit(ray, t, sr) && (t < tmin)) {
         tmin = t;
         sr.hit = true;
         sr.material = (*it)->getMaterial();
         sr.hitPoint = ray.origin + ray.direction * t;
         localHitPoint = sr.localHitPoint;
         normal = sr.normal;
      }
   }
   
   if(sr.hit) {
      sr.t = tmin;
      sr.normal = normal;
      sr.localHitPoint = localHitPoint;
   }
   
   return sr;
}
