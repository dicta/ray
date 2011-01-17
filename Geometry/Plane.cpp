#include "Plane.h"
#include "Parser/Hash.h"
#include "Parser/Value.h"
#include "Materials/Matte.h"
#include "Textures/PlaneChecker.h"

Plane::Plane() : point(Point3D(1, 0, 0)), normal(Vector3D(0, 0, 1)) {
}

bool Plane::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   double t = (point - ray.origin).dot(normal) / (ray.direction.dot(normal));
   
   if(t > epsilon) {
      tmin = t;
      sr.normal = normal;
      sr.localHitPoint = ray.origin + ray.direction * t;
      return true;
   }
   return false;
}

bool Plane::shadowHit(const Ray& ray, double& tmin) const {
   double t = (point - ray.origin).dot(normal) / (ray.direction.dot(normal));
   
   if(t > epsilon) {
      tmin = t;
      return true;
   }
   return false;
}

void Plane::setHash(Hash* hash) {
   Array* a = hash->getValue("point")->getArray();
   point.set(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
   
   a = hash->getValue("normal")->getArray();
   normal.set(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
   normal.normalize();
   
   setupMaterial(hash->getValue("material")->getHash());
}
