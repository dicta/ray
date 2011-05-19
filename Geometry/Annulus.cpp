#include "Annulus.h"
#include "Parser/Hash.h"
#include "Math/Maths.h"
#include "Materials/Material.h"

Annulus::Annulus() :
   GeometryObject(),
   center(),
   normal(),
   innerSquared(0),
   outerSquared(0),
   angleRange(false),
   minAngle(0),
   maxAngle(0)
{
}

Annulus::Annulus(const Point3D& c, const Vector3D& n, double inner, double outer) :
   GeometryObject(),
   center(c),
   normal(n),
   innerSquared(inner*inner),
   outerSquared(outer*outer),
   angleRange(false),
   minAngle(0),
   maxAngle(0)
{
}

void Annulus::setAngleRange(double min, double max) {
   angleRange = true;
   minAngle = min * DEG_TO_RAD;
   maxAngle = max * DEG_TO_RAD;
}

void Annulus::setHash(Hash* hash) {
   center.set(hash->getValue("center")->getArray());
   normal.set(hash->getValue("normal")->getArray());
   normal.normalize();

   double radius = hash->getDouble("innerRadius");
   innerSquared = radius * radius;

   radius = hash->getDouble("outerRadius");
   outerSquared = radius * radius;
   
   if(hash->contains("angleRange")) {
      angleRange = true;
      Array* a = hash->getValue("angleRange")->getArray();
      minAngle = a->at(0)->getDouble() * DEG_TO_RAD;
      maxAngle = a->at(1)->getDouble() * DEG_TO_RAD;
   }

   setupMaterial(hash->getValue("material")->getHash());
}

bool Annulus::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   double t = (center - ray.origin).dot(normal) / ray.direction.dot(normal);
   
   if(t < epsilon) {
      return false;
   }
   
   // Calculate the hit point
   Point3D p = ray(t);
   // Calculate distance from object center to hit point
   double dist = center.distanceSquared(p);
   
   // If distance is between outer and inner radius, it hits the annulus
   if(innerSquared <= dist && dist < outerSquared && partCheck(p)) {
      tmin = t;
      sr.normal = normal;
      sr.localHitPoint = p;
      sr.tu = (dist-innerSquared) / (outerSquared-innerSquared);
      sr.tv = 0;
      return true;
   }
   return false;
}

bool Annulus::partCheck(const Point3D& hit) const {
   if(!angleRange) {
      return true;
   }

   double phi = atan2(hit.x, hit.z);
   if(phi < 0.0) {
      phi += 2.0 * M_PI;
   }
   if(phi < minAngle || phi > maxAngle) {
      return false;
   }

   return true;
}

bool Annulus::shadowHit(const Ray& ray, double& tmin) const {
   double t = (center - ray.origin).dot(normal) / ray.direction.dot(normal);

   if(t < epsilon) {
      return false;
   }

   // Calculate the hit point
   Point3D p = ray.origin + ray.direction * t;
   // Calculate distance from object center to hit point
   double dist = center.distanceSquared(p);

   // If distance is between outer and inner radius, it hits the annulus
   if(innerSquared <= dist && dist < outerSquared && partCheck(p)) {
      ShadeRecord sr;
      sr.localHitPoint = p;
      sr.tu = (dist-innerSquared) / (outerSquared-innerSquared);
      sr.tv = 0;
      float alpha = material->getAlpha(sr, ray);

      if(alpha > 0.2) {
         tmin = t;
         return true;
      }
   }
   return false;
}
