#include "NoiseTexture.h"
#include "Parser/Hash.h"
#include "Noise/LinearNoise.h"
#include "Noise/CubicNoise.h"
#include "Math/Maths.h"

NoiseTexture::NoiseTexture() : Texture(), color(), noise(NULL), min(0.0), max(1.0), noiseType("") {
}

NoiseTexture::~NoiseTexture() {
   delete noise;
}

void NoiseTexture::setHash(Hash* hash) {
   color.set(hash->getValue("color")->getArray());
   
   string interp = hash->getString("interpolation");
   if(interp == "linear") {
      noise = new LinearNoise();
   }
   else if(interp == "cubic") {
      noise = new CubicNoise();
   }
   else {
      assert(false);
   }

   min = hash->getDouble("min");
   max = hash->getDouble("max");

   noiseType = hash->getString("noiseType");
   noise->setHash(hash);
}

Color NoiseTexture::getColor(const ShadeRecord& sr) const {
   float value = 0.0;
   
   if(noiseType == "fractal") {
      value = noise->fractalSum(sr.localHitPoint);
   }
   else if(noiseType == "turbulence") {
      value = noise->turbulence(sr.localHitPoint);
   }
   else if(noiseType == "fbm") {
      value = noise->fbm(sr.localHitPoint);
   }
   else {
      assert(false);
   }
   
   value = min + (max - min) * value;
   value = clamp(value, 0.0, 1.0);
   return color * value;
}
