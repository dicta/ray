#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "GeometryObject.h"

class Cylinder : public GeometryObject {
   
public:
   Cylinder();
   Cylinder(float r, float min, float max);
   
   void setAngleRange(double min, double max);
   
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
private:
   bool partCheck(const Ray& ray, double t) const;

   float radius;
   float minY;
   float maxY;
   
   bool rangeSet;
   double minAngle;
   double maxAngle;
};

#endif
