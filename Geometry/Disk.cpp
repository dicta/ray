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
#include "Samplers/MultiJittered.h"
#include <math.h>

Disk::Disk() : LightObject(), center(), normal(), radiusSquared(1) {
   sampler = new MultiJittered(100);
   sampler->mapSamplesToUnitDisk();
}

Disk::~Disk() {
   delete sampler;
}

void Disk::setHash(Hash* hash) {
   center.set(hash->getValue("center")->getArray());
   normal.set(hash->getValue("normal")->getArray());
   normal.normalize();

   radius = hash->getDouble("radius");
   radiusSquared = radius * radius;
   inverseArea = 1.0 / M_PI * radiusSquared;

   a = Vector3D(0, 0, 1).cross(normal);
   b = normal.cross(a);

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

Point3D Disk::sample(const Point3D& hitPoint) const {
   Point2D* point = sampler->sampleUnitDisk();
   return (center + a * radius * point->x + b * radius * point->y);
}

Vector3D Disk::getNormal(const Point3D& point) const {
   return normal;
}

double Disk::pdf(const ShadeRecord& sr) const {
   return inverseArea;
}
