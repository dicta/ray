#ifndef _ANNULUS_H_
#define _ANNULUS_H_

#include "GeometryObject.h"

class Annulus : public GeometryObject {

public:
   Annulus();
   Annulus(const Point3D& c, const Vector3D& n, double inner, double outer);
   virtual ~Annulus() {}
   
   void setAngleRange(double min, double max);
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
private:
   bool partCheck(const Point3D& hit) const;

   Point3D center;
   Vector3D normal;
   double innerSquared;
   double outerSquared;
   
   bool angleRange;
   double minAngle;
   double maxAngle;
};

#endif
