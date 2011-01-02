/*
 *  Ambient.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Ambient.h"
#include "Parser/Hash.h"

Ambient::Ambient() : Light(), ls(0.0), color(1, 1, 1) {
}

Vector3D Ambient::getLightDirection(ShadeRecord& sr) {
   return Vector3D();
}

void Ambient::setHash(Hash* hash) {
   ls = hash->getDouble("radiance");
   color.set(hash->getValue("color")->getArray());
}

Color Ambient::L(const ShadeRecord& sr) {
   return color * ls;
}
