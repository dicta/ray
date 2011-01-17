#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Ray.h"

class Hash;

/**
 * Hash values:
 *    - type <i>string</i> - Defines the type of material to use. (phong, reflective, matte)
 *
 * Material classes:
 *    - Matte
 *    - Phong
 *    - Reflective
 */
class Material {
   
public:
   virtual Color shade(ShadeRecord& sr, const Ray& ray) { return BLACK; }
   virtual Color areaLightShade(ShadeRecord& sr, const Ray& ray) { return BLACK; }
   virtual void setHash(Hash* hash) = 0;
   virtual Color getLe(const ShadeRecord& sr) const { return BLACK; }
};

#endif
