#ifndef _DIRECTION_H_
#define _DIRECTION_H_

#include "Light.h"
#include "Utility/Color.h"
#include "Math/Vector3D.h"

class Direction : public Light {

public:
   Direction();
   ~Direction();

   virtual void setHash(Hash* hash);
   virtual Vector3D getLightDirection(ShadeRecord& sr);
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr);

   virtual Color L(const ShadeRecord& sr);

private:
   float ls;
   Color color;
   Vector3D direction;
};

#endif
