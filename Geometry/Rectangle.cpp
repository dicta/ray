/*
 *  ;
 *  RayTracer
 *
 *  Created by Eric Saari on 12/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Rectangle.h"
#include "Parser/Hash.h"
#include "Samplers/MultiJittered.h"
#include "Math/Point2D.h"

Rectangle::Rectangle() : LightObject() {
   sampler = new MultiJittered(100);
}

Rectangle::Rectangle(const Point3D& o, const Vector3D& _a, const Vector3D& _b) :
   LightObject(),
   origin(o),
   a(_a),
   b(_b)
{
   sampler = new MultiJittered(100);
   setup();
}

Rectangle::~Rectangle() {
   delete sampler;
}

void Rectangle::setHash(Hash* hash) {
   origin.set(hash->getValue("origin")->getArray());
   a.set(hash->getValue("a")->getArray());
   b.set(hash->getValue("b")->getArray());
   setup();

   setupMaterial(hash->getValue("material")->getHash());
}

void Rectangle::setup() {
   normal = a.cross(b).normalize();

   lengthASquared = a.length() * a.length();
   lengthBSquared = b.length() * b.length();

   inverseArea = 1.0 / (a.length() * b.length());
   
   bbox.expand(origin);
   bbox.expand(origin + a);
   bbox.expand(origin + b);   
}

bool Rectangle::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   double t = (origin - ray.origin).dot(normal) / ray.direction.dot(normal);
   if(t <= epsilon) {
      return false;
   }

   Point3D p = ray(t);
   Vector3D d = p - origin;

   float ddota = d.dot(a);
   if(ddota < 0.0 || ddota > lengthASquared) {
      return false;
   }

   float ddotb = d.dot(b);
   if(ddotb < 0.0 || ddotb > lengthBSquared) {
      return false;
   }

   tmin = t;
   sr.normal = normal;
   sr.localHitPoint = p;
   sr.tu = ddota / lengthASquared;
   sr.tv = ddotb / lengthBSquared;

   return true;
}

bool Rectangle::shadowHit(const Ray& ray, double& tmin) const {
   if(ignoreShadowRays) {
      return false;
   }

   double t = (origin - ray.origin).dot(normal) / ray.direction.dot(normal);
   if(t <= epsilon) {
      return false;
   }

   Point3D p = ray.origin + ray.direction * t;
   Vector3D d = p - origin;

   float ddota = d.dot(a);
   if(ddota < 0.0 || ddota > lengthASquared) {
      return false;
   }

   float ddotb = d.dot(b);
   if(ddotb < 0.0 || ddotb > lengthBSquared) {
      return false;
   }

   tmin = t;

   return true;
}

Point3D Rectangle::sample(const Point3D& hitPoint) const {
   Point2D* point = sampler->sampleUnitSquare();
   return (origin + a * point->x + b * point->y);
}

Vector3D Rectangle::getNormal(const Point3D& point) const {
   return normal;
}

double Rectangle::pdf(const ShadeRecord& sr) const {
   return inverseArea;
}
