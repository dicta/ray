#ifndef _WOOD_TEXTURE_H_
#define _WOOD_TEXTURE_H_

#include "Texture.h"
#include "Utility/Color.h"

class LatticeNoise;

/**
 * Procedural texture for wood based on the Larry Gritz Renderman wood texture.
 */
class WoodTexture: public Texture {

public:
   WoodTexture();
   virtual ~WoodTexture();

   virtual void setHash(Hash* hash);
   virtual Color getColor(const ShadeRecord& sr) const;
   
private:
   LatticeNoise* noise;
   /** Color of lighter rings. */
   Color lightColor;
   /** Color of darker rings. */
   Color darkColor;
   /** Mean radial distance between rings. Smaller values result in larger rings. */
   double ringFreq;
   /** Amount od variation in ring spacing and thickness. */
   double ringUneveness;
   /** Used to perturb hit point. */
   double ringNoise;
   /** Used to perturb hit point. */
   double ringNoiseFreq;
   /** Perturbs the texture off the Y axis. */
   double trunkWobble;
   /** Perturbs the texture off the Y axis. */
   double trunkWobbleFreq;
   /** Distort rings so they are not perfectly round. This is noise magnitude. */
   double angularWobble;
   /** Distort rings so they are not perfectly round. This specifies how fast noise varies around the rings. */
   double angularWobbleFreq;
   /** Grain size. */
   double grainFreq;
   /** Weight of grain in shading, in range [0, 1]. 0 is not shaded, 1 is fully shaded. */
   double grainy;
   /** Weight of rings in shading, in range [0, 1]. 0 is not shaded, 1 is fully shaded. */
   double ringy;
};

#endif
