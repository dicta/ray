/*
 *  SkyTexture.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/22/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SkyTexture.h"
#include "Noise/CubicNoise.h"
#include "Math/Maths.h"
#include "Parser/Hash.h"

SkyTexture::SkyTexture() : Texture(), color1(), color2(), noise(NULL), min(0.0), max(1.0) {
}

SkyTexture::~SkyTexture() {
   delete noise;
}

void SkyTexture::setHash(Hash* hash) {
   color1.set(0.1, 0.2, 0.8);
   color2.set(1, 1, 1);   
   noise = new CubicNoise(hash->getInteger("seed"));
   min = hash->getDouble("min");
   max = hash->getDouble("max");
   
   noise->setHash(hash);
}

Color SkyTexture::getColor(const ShadeRecord& sr) const {
   float value = noise->fractalSum(sr.localHitPoint);
   
   value = min + (max - min) * value;
   value = clamp(value, 0.0, 1.0);
   
   return color1 * (1.0 - value) + color2 * value;
}
