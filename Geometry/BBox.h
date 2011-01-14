#ifndef _BBOX_H_
#define _BBOX_H_

#include "Math/Ray.h"

class BBox {

public:
   BBox();
   bool hit(const Ray& ray) const;
   void expand(const BBox& b);
   void expand(const Point3D& b);
   bool contains(const Point3D& p) const;
   
   double x0, y0, z0;
   double x1, y1, z1;
};

inline bool BBox::contains(const Point3D& p) const {
   return ((p.x > x0 && p.x < x1) && (p.y > y0 && p.y < y1) && (p.z > z0 && p.z < z1));
};

#endif
