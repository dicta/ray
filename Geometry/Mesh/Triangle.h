#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Geometry/GeometryObject.h"
#include "Math/Point3D.h"
#include "Math/Vector3D.h"

class Triangle : public GeometryObject {
   
public:
   Triangle(const Point3D& p1, const Point3D& p2, const Point3D& p3);
   virtual ~Triangle();
   
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
private:
   Point3D v0, v1, v2;
   Vector3D normal;
};

#endif
