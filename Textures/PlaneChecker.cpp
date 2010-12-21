/*
 *  PlaneChecker.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "PlaneChecker.h"
#include "Parser/Hash.h"
#include <math.h>

PlaneChecker::PlaneChecker() : color1(), color2(), size(1), borderColor(), borderWidth(0.0) {
}

Color PlaneChecker::getColor(const ShadeRecord& sr) const {
   float x = sr.localHitPoint.x;
   float z = sr.localHitPoint.z;
   int ix = (int) floor(x / size);
   int iz = (int) floor(z / size);
   
   bool inBorder = false;
   
   if(borderWidth > 0.0) {
      float fx = x / size - ix;
      float fz = z / size - iz;
      float width = 0.5 * borderWidth / size;
      inBorder = (fx < width || fx > 1.0 - width) || (fz < width) || (fz > 1.0 - width);
   }
   
   if((ix + iz) % 2 == 0 && !inBorder) {
      return color1;
   }
   else if(!inBorder) {
      return color2;
   }
   
   return borderColor;
}

void PlaneChecker::setHash(Hash* hash) {
   color1 = Color(hash->getValue("color1")->getArray());
   color2 = Color(hash->getValue("color2")->getArray());
   size = hash->getDouble("size");
   
   if(hash->contains("borderColor")) {
      borderColor.set(hash->getValue("borderColor")->getArray());
   }
   
   if(hash->contains("borderWidth")) {
      borderWidth = hash->getDouble("borderWidth");
   }
}
