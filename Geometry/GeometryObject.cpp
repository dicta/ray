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
#include "Materials/Reflective.h"
#include "Parser/Hash.h"
#include <math.h>
#include <string>

const double GeometryObject::epsilon = 1.0 * pow(10, -6);

GeometryObject::GeometryObject() : doDelete(true), material(NULL), ignoreShadow(false) {
}

GeometryObject::~GeometryObject() {
   if(material != NULL) {
      delete material;
   }
}

void GeometryObject::setupMaterial(Hash* hash) {
   string type = hash->getString("type");
   
   if(hash->contains("ignoreShadow")) {
      ignoreShadow = true;
   }
   
   if(type == "phong") {
      material = new Phong();
   }
   else if(type == "reflective") {
      material = new Reflective();
   }
   else {
      // Matte is the default type
      material = new Matte();
   }

   material->setHash(hash);
}

void GeometryObject::setMaterial(Material *m) { 
   material = m; 
}
