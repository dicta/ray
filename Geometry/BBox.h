#ifndef _BBOX_H_
#define _BBOX_H_

#include "Math/Ray.h"

class BBox {

public:
   BBox();
   bool hit(const Ray& ray) const;
   void expand(const BBox& b);
   void expand(const Point3D& b);
   
   double x0, y0, z0;
   double x1, y1, z1;
};

#endif
