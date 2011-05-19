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

Emissive::Emissive() {
   ls = 1.0;
}

void Emissive::setHash(Hash* hash) {
   ls = hash->getDouble("radiance");
   color.set(hash->getValue("color")->getArray());
}

Color Emissive::shade(ShadeRecord& sr, const Ray& ray) {
   return areaLightShade(sr, ray);
}

Color Emissive::areaLightShade(ShadeRecord& sr, const Ray& ray)  {
   if((sr.normal * -1.0).dot(ray.direction) > 0.0) {
      return color * ls;
   }
   return BLACK;
}

Color Emissive::getLe(const ShadeRecord& sr) const {
   return color * ls;
}

void Emissive::setColor(float r, float g, float b) {
   color.set(r, g, b);
}

void Emissive::setDiffuse(float d) {
}
