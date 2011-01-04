/*
 *  AreaLight.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AreaLight.h"
#include "Geometry/LightObject.h"
#include "Geometry/GeometryManager.h"
#include "Materials/Material.h"
#include "Materials/Emissive.h"
#include "Parser/Hash.h"

AreaLight::AreaLight() : Light() {
   material = new Emissive();
}

AreaLight::~AreaLight() {
   delete material;
}

Vector3D AreaLight::getLightDirection(ShadeRecord& sr) {
   sr.samplePoint = new Point3D(object->sample());
   sr.lightNormal = new Vector3D(object->getNormal(*sr.samplePoint));
   
   Vector3D wi = *sr.samplePoint - sr.hitPoint;
   wi.normalize();
   sr.wi = new Vector3D(wi);

   return wi;
}

void AreaLight::setHash(Hash* hash) {
   material->setHash(hash);
   
   if(hash->contains("rectangle")) {
      Hash* rect = hash->getValue("rectangle")->getHash();
      object = (LightObject*) GeometryManager::instance().createObject("rectangle", rect);
      object->ignoreShadowRays = true;
   }
   else if(hash->contains("disk")) {
      Hash* disk = hash->getValue("disk")->getHash();
      object = (LightObject*) GeometryManager::instance().createObject("disk", disk);
      object->ignoreShadowRays = true;
   }
}

bool AreaLight::inShadow(const Ray& ray, const ShadeRecord& sr) {
   double t;
   double ts = (*sr.samplePoint - ray.origin).dot(ray.direction);
      
   for(GeometryIter it = GeometryManager::instance().begin(); it != GeometryManager::instance().end(); it++) {
      if((*it)->shadowHit(ray, t) && (t < ts)) {
         return true;
      }
   }
   return false; 
}

Color AreaLight::L(const ShadeRecord& sr) {
   float ndotd = -(*sr.lightNormal).dot(*sr.wi);
   if(ndotd > 0.0) {
      return material->getLe(sr);
   }
   return BLACK; 
}

float AreaLight::G(const ShadeRecord& sr) {
   float ndotd = -(*sr.lightNormal).dot(*sr.wi);
   float d2 = sr.samplePoint->distanceSquared(sr.hitPoint);
   return ndotd / d2;
}

float AreaLight::pdf(const ShadeRecord& sr) { 
   return object->pdf(sr);
}
