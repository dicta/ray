#ifndef _SPOTLIGHT_H_
#define _SPOTLIGHT_H_

#include "Light.h"

/**
 * Hash values:
 * <ul>
 * <li>location [ x y z ] - Location of the light source
 * <li>direction [ x y z ] - Direction of the center of the spotlight cone
 * <li>color [ r g b ] - Color of the light
 * <li>radiance <i>double</i> - Light intensity
 * <li>width <i>double</i> - Angular width of light cone
 * <li>falloff <i>double</i> - Angular width of start of light falloff
 * </ul>
 */
class SpotLight : public Light {

public:
   SpotLight();
   virtual ~SpotLight() {}
   
   virtual Vector3D getLightDirection(ShadeRecord& sr);
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);
   virtual void setHash(Hash* hash);
   virtual Color L(const ShadeRecord& sr);

private:
   double falloff(const ShadeRecord& sr) const;

   float ls;
   Color color;
   double cosWidth;
   double cosFalloff;

   Point3D location;
   Vector3D direction;
};

#endif
