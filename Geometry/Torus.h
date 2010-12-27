#ifndef _TORUS_H_
#define _TORUS_H_

#include "GeometryObject.h"
#include "BBox.h"

class Torus : public GeometryObject {

public:
   Torus();
   virtual ~Torus() {}

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
private:
   void computeNormal(Vector3D& n, const Point3D& p) const;

   double a;
   double b;
   BBox bbox;
};

#endif
