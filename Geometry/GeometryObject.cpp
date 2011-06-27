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
#include "Parser/Hash.h"
#include "Textures/Texture.h"
#include <math.h>
#include <string>

const double GeometryObject::epsilon = 1.0 * pow(10, -6);

GeometryObject::GeometryObject() : doDelete(true), ignoreShadow(false), bbox(), material(NULL), normalMap(NULL) {
}

GeometryObject::~GeometryObject() {
   if(material != NULL) {
      delete material;
      material = NULL;
   }
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
      material = new Phong();
   }
   else if(type == "reflective") {
      material = new Reflective();
   }
   else if(type == "atmosphere") {
      material = new Atmosphere();
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

void GeometryObject::coordinateSystem(const Vector3D& v1, Vector3D* v2, Vector3D* v3) const {
   if(abs(v1.x) > abs(v1.y)) {
      double invLen = 1.0 / sqrt(v1.x * v1.x + v1.z * v1.z);
      *v2 = Vector3D(-v1.z * invLen, 0.0, v1.x * invLen);
   }
   else {
      double invLen = 1.0 / sqrt(v1.y * v1.y + v1.z * v1.z);
      *v2 = Vector3D(0.0, v1.z * invLen, -v1.y * invLen);
   }
   *v3 = v1.cross(*v2);
}
