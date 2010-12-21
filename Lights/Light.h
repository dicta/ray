#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Math/Vector3D.h"
#include "Math/Point3D.h"
#include "Utility/Color.h"
#include "Math/Ray.h"
#include "Utility/ShadeRecord.h"

class Hash;

class Light {
   
public:
   Light() {}
   virtual ~Light() {}
   
   virtual Vector3D getLightDirection(const Point3D& hitPoint) = 0;
   virtual void setHash(Hash* hash) = 0;
   virtual bool inShadow(const Ray& ray, const ShadeRecord& sr) { return false; }
   
   virtual Color L(const ShadeRecord& sr) { return BLACK; }
   virtual float G(const ShadeRecord& sr) { return 1.0; }
   virtual float pdf(const ShadeRecord& sr) { return 1.0; }
};

#endif
