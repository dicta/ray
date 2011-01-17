#include "Texture.h"
#include "Parser/Hash.h"
#include "PlaneChecker.h"
#include "NoiseTexture.h"
#include "SkyTexture.h"
#include "WoodTexture.h"
#include "GraniteTexture.h"
#include "MarbleTexture.h"

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
   else if(type == "wood") {
      texture = new WoodTexture();
   }
   else if(type == "granite") {
      texture = new GraniteTexture();
   }
   else if(type == "marble") {
      texture = new MarbleTexture();
   }
   else {
      assert(false);
   }
   texture->setHash(hash);

   return texture;
}
