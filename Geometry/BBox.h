#ifndef _BBOX_H_
#define _BBOX_H_

#include "Math/Ray.h"

class BBox {

public:
   BBox();
   bool hit(const Ray& ray) const;
   
   double x0, y0, z0;
   double x1, y1, z1;
};

#endif
