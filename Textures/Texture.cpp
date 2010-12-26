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
#include "NoiseTexture.h"
#include "SkyTexture.h"

Texture* Texture::createTexture(Hash* hash) {
   string type = hash->getString("type");
   Texture* texture;
   
   if(type == "planeChecker") {
      texture = new PlaneChecker();
   }
   else if(type == "noise") {
      texture = new NoiseTexture();
   }
   else if(type == "sky") {
      texture = new SkyTexture();
   }
   else {
      assert(false);
   }
   texture->setHash(hash);

   return texture;
}
