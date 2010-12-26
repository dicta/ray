/*
 *  Cylinder.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Cylinder.h"
#include "Parser/Hash.h"
#include <math.h>

Cylinder::Cylinder() : radius(1.0), minY(-1.0), maxY(1.0) {
}

void Cylinder::setHash(Hash* hash) {
   radius = hash->getDouble("radius");
   minY = hash->getDouble("minY");
   maxY = hash->getDouble("maxY");
   setupMaterial(hash->getValue("material")->getHash());
}

bool Cylinder::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   double a = pow(ray.direction.x, 2.0) + pow(ray.direction.z, 2.0);
   double b = 2.0 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z);
   double c = pow(ray.origin.x, 2.0) + pow(ray.origin.z, 2.0) - pow(radius, 2.0);
   double disc = b * b - 4.0 * a * c;
   
   if(disc < 0.0) {
      return false;
   }
   
   double e = sqrt(disc);
   double denom = 2.0 * a;
   double t = (-b - e) / denom;
   double yHit = ray.origin.y + ray.direction.y * t;
   
   if(t > epsilon && minY <= yHit && yHit <= maxY) {
      tmin = t;
      sr.localHitPoint = ray.origin + ray.direction * t;
      sr.normal.set(sr.localHitPoint.x / radius, 0.0, sr.localHitPoint.z / radius);
      
      if((-ray.direction).dot(sr.normal) < 0.0) {
         sr.normal *= -1;
      }
      return true;
   }
   
   t = (-b + e) / denom;
   yHit = ray.origin.y + ray.direction.y * t;

   if(t > epsilon && minY <= yHit && yHit <= maxY) {
      tmin = t;
      sr.localHitPoint = ray.origin + ray.direction * t;
      sr.normal.set(sr.localHitPoint.x / radius, 0.0, sr.localHitPoint.z / radius);
      
      if((-ray.direction).dot(sr.normal) < 0.0) {
         sr.normal *= -1;
      }
      return true;
   }
   
   return false;
}

bool Cylinder::shadowHit(const Ray& ray, double& tmin) const {
   double a = pow(ray.direction.x, 2.0) + pow(ray.direction.z, 2.0);
   double b = 2.0 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z);
   double c = pow(ray.origin.x, 2.0) + pow(ray.origin.z, 2.0) - pow(radius, 2.0);
   double disc = b * b - 4.0 * a * c;
   
   if(disc < 0.0) {
      return false;
   }
   
   double e = sqrt(disc);
   double denom = 2.0 * a;
   double t = (-b - e) / denom;
   double yHit = ray.origin.y + ray.direction.y * t;
   
   if(t > epsilon && minY <= yHit && yHit <= maxY) {
      tmin = t;
      return true;
   }
   
   t = (-b + e) / denom;
   yHit = ray.origin.y + ray.direction.y * t;

   if(t > epsilon && minY <= yHit && yHit <= maxY) {
      tmin = t;
      return true;
   }
   
   return false;
}
