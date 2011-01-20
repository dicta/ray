#ifndef _MARBLE_TEXTURE_H_
#define _MARBLE_TEXTURE_H_

#include "Texture.h"

class LatticeNoise;

/**
 * Hash values:
 *    - seed <i>integer</i> - Seed value for the random number generator
 *    - mainColor [ r g b ] - Main marble color
 *    - veinColor [ r g b ] - Color of marble veins
 *    - numOctaves <i>integer</i> - Number of lookups to perform in noise data. Each successive octave contributes to
 *                                  finer amounts of detail
 *    - lacunarity <i>double</i> - Optional. Defaults to 1
 *    - gain <i>double</i> - Optional, defaults to 1.0.
 */
class MarbleTexture : public Texture {
   
public:
   MarbleTexture();
   virtual ~MarbleTexture();
   
   virtual Color getColor(const ShadeRecord& sr) const;
   virtual void setHash(Hash* hash);
   
private:
   Color mainColor;
   Color veinColor;
   LatticeNoise* noise;
};

#endif
