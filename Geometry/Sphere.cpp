/*
 *  Sphere.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>
#include "Sphere.h"
#include "Math/Vector3D.h"
#include "Parser/Hash.h"
#include "Materials/Matte.h"

Sphere::Sphere() : center(0, 0, 0), radius(1.0) {
}

Sphere::Sphere(const Point3D& c, double r) : center(c), radius(r) {
}

bool Sphere::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   Vector3D temp(ray.origin - center);
   double a = ray.direction.dot(ray.direction);
   double b = 2.0 * temp.dot(ray.direction);
   double c = temp.dot(temp) - (radius * radius);
   double disc = b * b - 4.0 * a * c;
   
   if(disc < 0.0) {
      return false;
   }
   
   double e = sqrt(disc);
   double denom = 2.0 * a;
   double t = (-b - e) / denom;
   
   if(t > epsilon) {
      tmin = t;
      sr.normal = (temp + ray.direction * t) / radius;
      sr.normal.normalize();
      sr.localHitPoint = ray.origin + ray.direction * t;
      return true;
   }
   
   t = (-b + e) / denom;
   if(t > epsilon) {
      tmin = t;
      sr.normal = (temp + ray.direction * t) / radius;
      sr.normal.normalize();
      sr.localHitPoint = ray.origin + ray.direction * t;
      return true;
   }
   
   return false;
}

bool Sphere::shadowHit(const Ray& ray, double& tmin) const {
   Vector3D temp(ray.origin - center);
   double a = ray.direction.dot(ray.direction);
   double b = 2.0 * temp.dot(ray.direction);
   double c = temp.dot(temp) - (radius * radius);
   double disc = b * b - 4.0 * a * c;
   
   if(disc < 0.0) {
      return false;
   }
   
   double e = sqrt(disc);
   double denom = 2.0 * a;
   double t = (-b - e) / denom;
   
   if(t > epsilon) {
      tmin = t;
      return true;
   }
   
   t = (-b + e) / denom;
   if(t > epsilon) {
      tmin = t;
      return true;
   }
   
   return false;
}

void Sphere::setHash(Hash* hash) {
   center.set(hash->getValue("center")->getArray());
   radius = hash->getDouble("radius");
   setupMaterial(hash->getValue("material")->getHash());
}
