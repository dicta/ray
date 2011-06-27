#ifndef _MATTE_H_
#define _MATTE_H_

#include "Material.h"

class Lambertian;
class Texture;
class Color;

/**
 * Hash values:
 * <ul>
 * <li>ka <i>val</i> - Ambient contibution
 * <li>kd <i>val</i> - Diffuse contribution
 * <li>color [ r g b ] - Optional material color
 * <li>texture {} - Optional texture hash. See Texture.
 * </ul>
 * If neither color or texture is specified, material defaults to black.
 */
class Matte : public Material {

public:
   Matte();
   virtual ~Matte();

   virtual Color shade(ShadeRecord& sr, const Ray& ray);
   virtual Color areaLightShade(ShadeRecord& sr, const Ray& ray);
   virtual void setHash(Hash* hash);
   virtual float getAlpha(const ShadeRecord& sr, const Ray& ray) const;

   virtual void setColor(float r, float g, float b);
   virtual void setDiffuse(float d);
   virtual void setTexture(string texture);

   void setAmbientColor(Color* c);
   void setDiffuseColor(Color* c);

private:
   Lambertian* ambientBRDF;
   Lambertian* diffuseBRDF;
};

#endif
