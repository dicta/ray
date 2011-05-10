#include "BBox.h"
#include "GeometryObject.h"

BBox::BBox() : x0(1e6), y0(1e6), z0(1e6), x1(-1e6), y1(-1e6), z1(-1e6) {
}

bool BBox::hit(const Ray& ray) const {
   double t0, t1;
   return hit(ray, t0, t1);
}

bool BBox::hit(const Ray& ray, double& hitt0, double& hitt1) const {
   double t0 = 0;
   double t1 = 1.0E10;

   if(!slabHit(x0, x1, ray.origin.x, ray.direction.x, t0, t1)) return false;
   if(!slabHit(y0, y1, ray.origin.y, ray.direction.y, t0, t1)) return false;
   if(!slabHit(z0, z1, ray.origin.z, ray.direction.z, t0, t1)) return false;

   hitt0 = t0;
   hitt1 = t1;
   return true;
}

bool BBox::slabHit(double b0, double b1, double ro, double rd, double& t0, double& t1) const {
   double near = (b0 - ro) / rd;
   double far = (b1 - ro) / rd;
   if(near > far) swap(near, far);
   t0 = near > t0 ? near : t0;
   t1 = far < t1 ? far : t1;
   
   if(t0 > t1) {
      return false;
   }
   return true;
}

void BBox::expand(const BBox& b) {
   x0 = min(x0, b.x0);
   y0 = min(y0, b.y0);
   z0 = min(z0, b.z0);
   
   x1 = max(x1, b.x1);
   y1 = max(y1, b.y1);
   z1 = max(z1, b.z1);
   
   wx = x1 - x0;
   wy = y1 - y0;
   wz = z1 - z0;
}

void BBox::expand(const Point3D& b) {
   x0 = min(x0, b.x); // - GeometryObject::epsilon);
   y0 = min(y0, b.y); // - GeometryObject::epsilon);
   z0 = min(z0, b.z); // - GeometryObject::epsilon);
   
   x1 = max(x1, b.x); // + GeometryObject::epsilon);
   y1 = max(y1, b.y); // + GeometryObject::epsilon);
   z1 = max(z1, b.z); // + GeometryObject::epsilon);
   
   wx = x1 - x0;
   wy = y1 - y0;
   wz = z1 - z0;
}

double BBox::maxExtent() const {
   if(wx > wy && wx > wz) {
      return wx;
   }
   else if(wy > wz) {
      return wy;
   }
   else {
      return wz;
   }
}
