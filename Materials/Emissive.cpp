/*
 *  Emissive.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Emissive.h"
#include "Parser/Hash.h"

void Emissive::setHash(Hash* hash) {
   ls = hash->getDouble("radiance");
   color.set(hash->getValue("color")->getArray());
}

Color Emissive::shade(const ShadeRecord& sr, const Ray& ray) {
   return areaLightShade(sr, ray);
}

Color Emissive::areaLightShade(const ShadeRecord& sr, const Ray& ray)  {
   if((sr.normal * -1.0).dot(ray.direction) > 0.0) {
      return color * ls;
   }
   return BLACK;
}

Color Emissive::getLe(const ShadeRecord& sr) const {
   return color * ls;
}
