/*
 *  SkyTexture.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/22/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SKY_TEXTURE_H_
#define _SKY_TEXTURE_H_

#include "Texture.h"

class LatticeNoise;

class SkyTexture : public Texture {
   
public:
   SkyTexture();
   virtual ~SkyTexture();
   
   virtual Color getColor(const ShadeRecord& sr) const;
   virtual void setHash(Hash* hash);
   
private:
   Color color1;
   Color color2;
   LatticeNoise* noise;
   float min, max;
};

#endif
