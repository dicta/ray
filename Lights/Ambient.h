#ifndef _AMBIENT_H_
#define _AMBIENT_H_

#include "Light.h"

class Ambient : public Light {
   
public:
   Ambient();
   virtual ~Ambient() {}
   
   virtual Vector3D getLightDirection(ShadeRecord& sr);
   virtual void setHash(Hash* hash);
   virtual Color L(const ShadeRecord& sr);
   
protected:
   float ls;
   Color color;
};

#endif
