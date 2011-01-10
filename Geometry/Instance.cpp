#include "Instance.h"
#include "Parser/Hash.h"
#include "GeometryManager.h"

Instance::Instance() : object(NULL), invMatrix(), fwdMatrix() {
}

Instance::Instance(GeometryObject* obj) : object(obj), invMatrix(), fwdMatrix() {
}

Instance::~Instance() {
   delete object;
}

void Instance::setHash(Hash* hash) {
   Hash* objHash = hash->getValue("object")->getHash();
   string type = objHash->getString("type");

   object = GeometryManager::instance().createObject(type, objHash, false);
   material = object->getMaterial();

   Array* transforms = hash->getValue("transforms")->getArray();
   int idx = 0;
   while(idx < transforms->size()) {
      type = transforms->at(idx)->getString();
      idx++;
      
      if(type == "translate") {
         Array* a = transforms->at(idx)->getArray();
         invMatrix.invTranslate(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
         fwdMatrix.translate(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
      }
      else if(type == "scale") {
         Array* a = transforms->at(idx)->getArray();
         invMatrix.invScale(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
      }
      else if(type == "rotateX") {
         invMatrix.invRotateX(transforms->at(idx)->getDouble());
         fwdMatrix.rotateX(transforms->at(idx)->getDouble());
      }
      else if(type == "rotateY") {
         invMatrix.invRotateY(transforms->at(idx)->getDouble());
      }
      else if(type == "rotateZ") {
         invMatrix.invRotateZ(transforms->at(idx)->getDouble());
      }

      idx++;
   }
}

bool Instance::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   Ray invRay;
   invRay.origin = invMatrix * ray.origin;
   invRay.direction = invMatrix * ray.direction;
   
   if(object->hit(invRay, tmin, sr)) {
      sr.normal = invMatrix.transformNormal(sr.normal);
      sr.normal.normalize();
      sr.localHitPoint = ray.origin + ray.direction * tmin;
      return true;
   }
   
   return false;
}

bool Instance::shadowHit(const Ray& ray, double& tmin) const {
   Ray invRay;
   invRay.origin = invMatrix * ray.origin;
   invRay.direction = invMatrix * ray.direction;
   return object->shadowHit(invRay, tmin);
}
