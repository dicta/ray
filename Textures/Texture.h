#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Utility/ShadeRecord.h"
#include "Utility/Color.h"
#include "Math/Point3D.h"

class Hash;

/**
 * Hash values:<br>
 * type <i>string</i> - Type of texture.<br>
 *
 * Possible types are planeChecker, noise, sky, wood, granite, marble, image
 */
class Texture {

public:
   Texture() {}
   virtual ~Texture() {}

   virtual Color getColor(const ShadeRecord& sr) const = 0;
   virtual void setHash(Hash* hash) = 0;
   virtual float getAlpha(const ShadeRecord& sr) const { return 1.0; }

   static Texture* createTexture(Hash* hash);
};

#endif
