/*
 *  Disk.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Disk.h"
#include "Parser/Hash.h"

Disk::Disk() : GeometryObject(), center(), normal(), radiusSquared(1) {
}

void Disk::setHash(Hash* hash) {
   center.set(hash->getValue("center")->getArray());
   normal.set(hash->getValue("normal")->getArray());
   normal.normalize();
   float radius = hash->getDouble("radius");
   radiusSquared = radius * radius;
   
   setupMaterial(hash->getValue("material")->getHash());
}

bool Disk::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   float t = (center - ray.origin).dot(normal) / ray.direction.dot(normal);
   
   if(t < epsilon) {
      return false;
   }
   
   Point3D p = ray.origin + ray.direction * t;
   if(center.distanceSquared(p) < radiusSquared) {
      tmin = t;
      sr.normal = normal;
      sr.localHitPoint = p;
      return true;
   }
   return false;
}

bool Disk::shadowHit(const Ray& ray, double& tmin) const {
   float t = (center - ray.origin).dot(normal) / ray.direction.dot(normal);
   
   if(t < epsilon) {
      return false;
   }
   
   Point3D p = ray.origin + ray.direction * t;
   if(center.distanceSquared(p) < radiusSquared) {
      tmin = t;
      return true;
   }
   return false;
}
