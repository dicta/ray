#ifndef _NOISE_TEXTURE_H_
#define _NOISE_TEXTURE_H_

#include "Texture.h"
#include <string>

using namespace std;

class LatticeNoise;

class NoiseTexture : public Texture {

public:
   NoiseTexture();
   virtual ~NoiseTexture();

   virtual Color getColor(const ShadeRecord& sr) const;
   virtual void setHash(Hash* hash);
   
private:
   Color color;
   LatticeNoise* noise;
   float min, max;
   string noiseType;
};

#endif
