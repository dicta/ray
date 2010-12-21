/*
 *  GeometryObject.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GeometryObject.h"
#include "Materials/Matte.h"
#include "Materials/Phong.h"
#include "Parser/Hash.h"
#include <math.h>
#include <string>

const double GeometryObject::epsilon = 1.0 * pow(10, -6);

GeometryObject::GeometryObject() {
}

GeometryObject::~GeometryObject() {
   delete material;
}

void GeometryObject::setupMaterial(Hash* hash) {
   string type = hash->getString("type");
   
   if(type == "phong") {
      material = new Phong();
   }
   else {
      // Matte is the default type
      material = new Matte();
   }

   material->setHash(hash);
}
