#ifndef _GRANITE_TEXTURE_H_
#define _GRANITE_TEXTURE_H_

#include "Texture.h"
#include "Utility/Color.h"

class LatticeNoise;

class GraniteTexture : public Texture {
   
public:
   GraniteTexture();
   virtual ~GraniteTexture();
   
   virtual Color getColor(const ShadeRecord& sr) const;
   virtual void setHash(Hash* hash);
   
private:
   LatticeNoise* noise;
   Color color;
};

#endif
