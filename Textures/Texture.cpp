/*
 *  Texture.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Texture.h"
#include "Parser/Hash.h"
#include "PlaneChecker.h"

Texture* Texture::createTexture(Hash* hash) {
   string type = hash->getString("type");
   Texture* texture;
   
   if(type == "planeChecker") {
      texture = new PlaneChecker();
   }
   texture->setHash(hash);

   return texture;
}
