#ifndef _BOX_H_
#define _BOX_H_

#include "GeometryObject.h"

/**
 * Creates a axis aligned box.
 *
 * Hash values:
 * <ul>
 * <li>p0 - Minimum corner. Array of doubles of the form [ x y z ]
 * <li>p1 - Maximum corner. Array of doubles of the form [ x y z ]
 * </ul>
 */
class Box : public GeometryObject {

public:
   Box();
   Box(Point3D& _p0, Point3D& _p1) : GeometryObject(), p0(_p0), p1(_p1) {}
   virtual ~Box() {}
   
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
private:
   void getNormal(Vector3D& n, const int face) const;

   Point3D p0;
   Point3D p1;
};

#endif
