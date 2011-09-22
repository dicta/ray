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
#include "Materials/Atmosphere.h"
#include "Materials/Emissive.h"
#include "Parser/Hash.h"
#include "Textures/Texture.h"
#include <math.h>
#include <string>

const double GeometryObject::epsilon = 1.0 * pow(10, -6);

GeometryObject::GeometryObject() : doDelete(true), ignoreShadow(false), bbox(), normalMap(NULL) {
}

GeometryObject::~GeometryObject() {
}

void GeometryObject::setupMaterial(Hash* hash) {
   string type = hash->getString("type");

   if(hash->contains("ignoreShadow")) {
      ignoreShadow = true;
   }

   if(hash->contains("normalMap")) {
      normalMap = Texture::createTexture(hash->getValue("normalMap")->getHash());
   }

   if(type == "phong") {
      material = shared_ptr<Material>(new Phong());
   }
   else if(type == "reflective") {
      material = shared_ptr<Material>(new Reflective());
   }
   else if(type == "atmosphere") {
      material = shared_ptr<Material>(new Atmosphere());
   }
   else if(type == "emissive") {
      material = shared_ptr<Material>(new Emissive());
   }
   else {
      // Matte is the default type
      material = shared_ptr<Material>(new Matte());
   }

   material->setHash(hash);
}

void GeometryObject::setMaterial(shared_ptr<Material> m) {
   material = m;
}
