#ifndef _PLANE_H_
#define _PLANE_H_

#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "GeometryObject.h"

class Hash;

/**
 * Hash values:
 * <ul>
 * <li>point [ x y z ] - Point on plane
 * <li>normal [ x y z ] - Normal to the plane
 * <li>material {} - Material hash. See the Material class.
 * </ul>
 */
class Plane : public GeometryObject {
   
public:
   Plane();

   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   virtual void setHash(Hash* hash);

private:
   Point3D point;
   Vector3D normal;
};

#endif
