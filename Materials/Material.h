#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <string>

#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Ray.h"

class Hash;
class Texture;

using namespace std;

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
   Material();
   virtual ~Material();

   virtual Color shade(ShadeRecord& sr, const Ray& ray) { return BLACK; }
   virtual void setHash(Hash* hash) = 0;
   virtual Color getLe(const ShadeRecord& sr) const { return BLACK; }
   virtual float getAlpha(const ShadeRecord& sr, const Ray& ray) const { return 1.0; }

   virtual void setColor(float r, float g, float b) = 0;
   virtual void setDiffuse(float d) = 0;
   virtual void setTexture(string texture) {}
   virtual void setTexture(Texture* tex) {}

   void setNormalMap(string texName);
   void applyNormalMap(ShadeRecord& sr);

private:
   Texture* normalMap;
};

#endif
