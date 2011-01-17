#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Utility/ShadeRecord.h"
#include "Utility/Color.h"

class Hash;

/**
 * Hash values:<br>
 * type <i>string</i> - Type of texture.<br>
 *
 * Possible types are planeChecker, noise, sky, wood, granite, marble
 */
class Texture {

public:
   virtual ~Texture() {}

   virtual Color getColor(const ShadeRecord& sr) const = 0;
   virtual void setHash(Hash* hash) = 0;
   
   static Texture* createTexture(Hash* hash);
};

#endif
