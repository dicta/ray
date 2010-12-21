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

Vector3D AreaLight::getLightDirection(const ShadeRecord& sr) {
   samplePoint = object->sample();
   lightNormal = object->getNormal(samplePoint);
   wi = samplePoint - sr.localHitPoint;
   wi.normalize();
   
   return wi;
}

void AreaLight::setHash(Hash* hash) {
   material->setHash(hash);
   
   Hash* rect = hash->getValue("rectangle")->getHash();
   object = (LightObject*) GeometryManager::instance().createObject("rectangle", rect);
   object->ignoreShadowRays = true;
}

bool AreaLight::inShadow(const Ray& ray, const ShadeRecord& sr) {
   double t;
   double ts = (samplePoint - ray.origin).dot(ray.direction);
   
   for(GeometryIter it = GeometryManager::instance().begin(); it != GeometryManager::instance().end(); it++) {
      if((*it)->shadowHit(ray, t) && (t < ts)) {
         return true;
      }
   }
   return false; 
}

Color AreaLight::L(const ShadeRecord& sr) {
   float ndotd = (lightNormal * -1).dot(wi);
   if(ndotd > 0.0) {
      return material->getLe(sr);
   }
   return BLACK; 
}

float AreaLight::G(const ShadeRecord& sr) {
   float ndotd = (lightNormal * -1.0).dot(wi);
   float d2 = samplePoint.distanceSquared(sr.localHitPoint);
   return ndotd / d2;
}

float AreaLight::pdf(const ShadeRecord& sr) { 
   return object->pdf(sr);
}
