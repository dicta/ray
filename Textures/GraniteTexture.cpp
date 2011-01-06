#include "GraniteTexture.h"
#include "Noise/CubicNoise.h"
#include "Parser/Hash.h"

GraniteTexture::GraniteTexture() : Texture(), noise(NULL), color(0.8, 0.8, 0.8) {
}

GraniteTexture::~GraniteTexture() {
   delete noise;
}

Color GraniteTexture::getColor(const ShadeRecord& sr) const {
   float value = noise->fractalSum(sr.localHitPoint);
   value = min(1.0, value * 18.0);
   return color * value;
}

void GraniteTexture::setHash(Hash* hash) {
   noise = new CubicNoise(hash->getInteger("seed"));
   noise->setHash(hash);
}
