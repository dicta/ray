#include "BBox.h"
#include "GeometryObject.h"

BBox::BBox() : x0(0), y0(0), z0(0), x1(0), y1(0), z1(0) {
}

bool BBox::hit(const Ray& ray) const {
   double ox = ray.origin.x;     double oy = ray.origin.y;     double oz = ray.origin.z;
   double dx = ray.direction.x;  double dy = ray.direction.y;  double dz = ray.direction.z;
   
   double txMin, tyMin, tzMin;
   double txMax, tyMax, tzMax;
   
   double a = 1.0 / dx;
   if(a >= 0.0) {
      txMin = (x0 - ox) * a;
      txMax = (x1 - ox) * a;
   }
   else {
      txMin = (x1 - ox) * a;
      txMax = (x0 - ox) * a;
   }
   
   double b = 1.0 / dy;
   if(b >= 0.0) {
      tyMin = (y0 - oy) * b;
      tyMax = (y1 - oy) * b;
   }
   else {
      tyMin = (y1 - oy) * b;
      tyMax = (y0 - oy) * b;
   }

   double c = 1.0 / dz;
   if(c >= 0.0) {
      tzMin = (z0 - oz) * c;
      tzMax = (z1 - oz) * c;
   }
   else {
      tzMin = (z1 - oz) * c;
      tzMax = (z0 - oz) * c;
   }

   double t0 = max(max(txMin, tyMin), tzMin);
   double t1 = min(min(txMax, tyMax), tzMax);
   
   return (t0 < t1 && t1 > GeometryObject::epsilon);
}
