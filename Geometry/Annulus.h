#ifndef _ANNULUS_H_
#define _ANNULUS_H_

#include "GeometryObject.h"

class Annulus : public GeometryObject {

public:
   Annulus();
   virtual ~Annulus() {}
   
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
private:
   Point3D center;
   Vector3D normal;
   float innerSquared;
   float outerSquared;
};

#endif
