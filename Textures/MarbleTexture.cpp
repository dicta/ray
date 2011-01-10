#include "MarbleTexture.h"
#include "Noise/CubicNoise.h"
#include "Math/Maths.h"
#include "Parser/Hash.h"

MarbleTexture::MarbleTexture() : Texture(), noise(NULL), mainColor(1.0, 1.0, 1.0), veinColor(0.1, 0.1, 0.1) {
}

MarbleTexture::~MarbleTexture() {
   delete noise;
}

Color MarbleTexture::getColor(const ShadeRecord& sr) const {
   double value = noise->turbulence(sr.localHitPoint);
   double t = 0.5 + 0.5 * sin(value);
   return mix(veinColor, mainColor, t);
}

void MarbleTexture::setHash(Hash* hash) {
   noise = new CubicNoise(hash->getInteger("seed"));
   noise->setHash(hash);
   mainColor.set(hash->getValue("mainColor")->getArray());
   veinColor.set(hash->getValue("veinColor")->getArray());
}
