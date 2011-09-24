/*
 *  AmbientOccluder.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AmbientOccluder.h"
#include "Samplers/MultiJittered.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Parser/Hash.h"
#include "Storage/Storage.h"

AmbientOccluder::AmbientOccluder() : Ambient(), minColor(), sampler(NULL) {
}

AmbientOccluder::~AmbientOccluder() {
   delete sampler;
}

Vector3D AmbientOccluder::getLightDirection(ShadeRecord& sr) {
   Point3D* sp = sampler->sampleHemisphere();
   return u * sp->x + v * sp->y + w * sp->z;
}

void AmbientOccluder::setHash(Hash* hash) {
   ls = hash->getDouble("radiance");
   color.set(hash->getValue("color")->getArray());
   minColor.set(hash->getValue("minColor")->getArray());
   
   int numSamples = hash->getInteger("numSamples");
   sampler = new MultiJittered(numSamples);
   sampler->mapSamplesToHemisphere(1);
}

bool AmbientOccluder::inShadow(const Ray& ray, const ShadeRecord& sr) {
   double t;
   
   if(GeometryManager::instance().getStorage()->shadowHit(ray, t)) {
      return true;
   }
/*
   for(GeometryIter it = GeometryManager::instance().begin(); it != GeometryManager::instance().end(); it++) {
      if((*it)->shadowHit(ray, t)) {
         return true;
      }
   }
*/
   return false;
}

Color AmbientOccluder::L(const ShadeRecord& sr) {
   w = sr.normal;
   v = w.cross(0.0072, 1.0, 0.0034);
   v.normalize();
   u = v.cross(w);
   
   Ray shadowRay;
   shadowRay.origin = sr.hitPoint;
   
   Point3D* sp = sampler->sampleHemisphere();
   shadowRay.direction = u * sp->x + v * sp->y + w * sp->z;
   
   if(inShadow(shadowRay, sr)) {
      return minColor * color * ls;
   }
   return color * ls;
}
