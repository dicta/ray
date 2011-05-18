/*
 *  PositionLight.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "PositionLight.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Math/Maths.h"

PositionLight::PositionLight() : Light(), ls(1.0), color(1, 1, 1), location() {
}

PositionLight::PositionLight(const Point3D& loc) : Light(), ls(1.0), color(1, 1, 1), location(loc) {
}

Vector3D PositionLight::getLightDirection(ShadeRecord& sr) {
//   Point3D newLoc;
//   newLoc.x = location.x + 3.0 * (2.0 * rand_float() - 1.0);
//   newLoc.y = location.y + 3.0 * (2.0 * rand_float() - 1.0);
//   newLoc.z = location.z + 3.0 * (2.0 * rand_float() - 1.0);
//   return (newLoc - hitPoint).normalize();
   return (location - sr.hitPoint).normalize();
}

bool PositionLight::inShadow(const Ray& ray, const ShadeRecord& sr) {
   double t;
   double d = location.distance(ray.origin);
   
   for(GeometryIter it = GeometryManager::instance().begin(); it != GeometryManager::instance().end(); it++) {
      if(!(*it)->ignoreShadow && (*it)->shadowHit(ray, t) && (t < d)) {
         return true;
      }
   }

   return false;
}

void PositionLight::setHash(Hash* hash) {
   location.set(hash->getValue("location")->getArray());
   color.set(hash->getValue("color")->getArray());
   ls = hash->getDouble("radiance");
}

Color PositionLight::L(const ShadeRecord& sr) {
   return color * ls;
}
