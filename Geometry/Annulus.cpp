#include "Annulus.h"
#include "Parser/Hash.h"

Annulus::Annulus() : GeometryObject(), center(), normal(), innerSquared(0), outerSquared(0) {
}

void Annulus::setHash(Hash* hash) {
   center.set(hash->getValue("center")->getArray());
   normal.set(hash->getValue("normal")->getArray());
   normal.normalize();

   float radius = hash->getDouble("innerRadius");
   innerSquared = radius * radius;

   radius = hash->getDouble("outerRadius");
   outerSquared = radius * radius;

   setupMaterial(hash->getValue("material")->getHash());
}

bool Annulus::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   float t = (center - ray.origin).dot(normal) / ray.direction.dot(normal);
   
   if(t < epsilon) {
      return false;
   }
   
   // Calculate the hit point
   Point3D p = ray.origin + ray.direction * t;
   // Calculate distance from object center to hit point
   double dist = center.distanceSquared(p);
   
   // If distance is between outer and inner radius, it hits the annulus
   if(innerSquared <= dist && dist < outerSquared) {
      tmin = t;
      sr.normal = normal;
      sr.localHitPoint = p;
      return true;
   }
   return false;
}

bool Annulus::shadowHit(const Ray& ray, double& tmin) const {
   float t = (center - ray.origin).dot(normal) / ray.direction.dot(normal);

   if(t < epsilon) {
      return false;
   }

   // Calculate the hit point
   Point3D p = ray.origin + ray.direction * t;
   // Calculate distance from object center to hit point
   double dist = center.distanceSquared(p);

   // If distance is between outer and inner radius, it hits the annulus
   if(innerSquared <= dist && dist < outerSquared) {
      tmin = t;
      return true;
   }
   return false;
}
