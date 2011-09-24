#include "AreaLight.h"
#include "Geometry/LightObject.h"
#include "Geometry/GeometryManager.h"
#include "Materials/Material.h"
#include "Materials/Emissive.h"
#include "Parser/Hash.h"
#include "Storage/Storage.h"

AreaLight::AreaLight() : Light() {
   material = new Emissive();
}

AreaLight::~AreaLight() {
   delete material;
}

Vector3D AreaLight::getLightDirection(ShadeRecord& sr) {
   sr.samplePoint = object->sample(sr.hitPoint);
   sr.lightNormal = object->getNormal(sr.samplePoint);

   sr.wi = sr.samplePoint - sr.hitPoint;
   sr.wi.normalize();

   return sr.wi;
}

void AreaLight::setHash(Hash* hash) {
   material->setHash(hash);

   if(hash->contains("rectangle")) {
      Hash* rect = hash->getValue("rectangle")->getHash();
      object = (LightObject*) GeometryManager::instance().createObject("rectangle", rect);
      object->ignoreShadow = true;
   }
   else if(hash->contains("disk")) {
      Hash* disk = hash->getValue("disk")->getHash();
      object = (LightObject*) GeometryManager::instance().createObject("disk", disk, false);
      object->ignoreShadow = true;
   }
   else if(hash->contains("sphere")) {
      Hash* sphere = hash->getValue("sphere")->getHash();
      object = (LightObject*) GeometryManager::instance().createObject("sphere", sphere);
      object->ignoreShadow = true;
   }

   if(hash->contains("numLightSamples")) {
      numLightSamples = hash->getInteger("numLightSamples");
   }
}

bool AreaLight::inShadow(const Ray& ray, const ShadeRecord& sr) {
   double t;
   double ts = (sr.samplePoint - ray.origin).dot(ray.direction);

   if(GeometryManager::instance().getStorage()->shadowHit(ray, t) && (t < ts)) {
      return true;
   }
/*
   for(GeometryIter it = GeometryManager::instance().begin(); it != GeometryManager::instance().end(); it++) {
      if((*it)->shadowHit(ray, t) && (t < ts)) {
         return true;
      }
   }
*/
   return false;
}

Color AreaLight::L(const ShadeRecord& sr) {
   float ndotd = -(sr.lightNormal).dot(sr.wi);
   if(ndotd > 0.0) {
      return material->getLe(sr);
   }
   return BLACK;
}

float AreaLight::G(const ShadeRecord& sr) {
//   float ndotd = sr.lightNormal.dot(-sr.wi);
//   float d2 = sr.samplePoint.distanceSquared(sr.hitPoint);
//   return ndotd / d2;
   return 1.0;
}

float AreaLight::pdf(const ShadeRecord& sr) {
//   return object->pdf(sr);
return 1.0;
}
