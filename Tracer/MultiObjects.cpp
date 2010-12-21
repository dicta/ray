/*
 *  MultiObjects.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>
#include "MultiObjects.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"

Color MultiObjects::traceRay(const Ray& ray) {
   double t;
   double tmin = 1.7 * pow(10, 308);
   Color color;
   color = LightManager::instance().getAmbientLight(sr);
   
   for(GeometryIter it = GeometryManager::instance().begin(); it != GeometryManager::instance().end(); it++) {
      if((*it)->hit(ray, t, sr) && (t < tmin)) {
         tmin = t;
         color = BLACK * LightManager::instance().getAmbientLight(sr);
         
         for(LightIter lit = LightManager::instance().begin(); lit != LightManager::instance().end(); lit++) {
            Vector3D lightDir = (*lit)->getLightDirection(sr);
            float ndoti = sr.normal.dot(lightDir);
            if(ndoti > 0.0) {
               color += BLACK * (*lit)->L(sr) * ndoti;
            }
         }
      }
   }
   
   color.normalize();
   return color;
}
