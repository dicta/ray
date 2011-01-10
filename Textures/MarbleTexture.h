#ifndef _MARBLE_TEXTURE_H_
#define _MARBLE_TEXTURE_H_

#include "Texture.h"

class LatticeNoise;

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
