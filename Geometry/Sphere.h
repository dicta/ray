#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "LightObject.h"
#include "Math/Point3D.h"

class Sampler;

/**
 * Creates a sphere or a part sphere.
 *
 * Hash values:
 *    - center [ x y z ] - Center of the sphere
 *    - radius <i>value</i> - Sphere radius in world coordinates
 *    - thetaRange [ min max ] - Optional Latitude range to include. Array of 2 daubles from 0 to 180. 0 is at the top of the sphere.
 *    - phiRange [ min max ] - Optional Longitude range to include. Array of 2 doubles from 0 to 360. 0 is in the positive Z axis.
 */
class Sphere : public LightObject {

public:
   Sphere();
   Sphere(const Point3D& c, double r);
   virtual ~Sphere();

   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   virtual void setHash(Hash* hash);

   virtual Point3D sample(const Point3D& hitPoint) const;
   virtual Vector3D getNormal(const Point3D& point) const;
   virtual double pdf(const ShadeRecord& sr) const;

private:
   bool partCheck(const Ray& ray, double t) const;
   void getNormalFromMap(ShadeRecord& sr) const;

   Point3D center;
   double radius;
   bool thetaRange, phiRange;
   double cosThetaMin, cosThetaMax;
   double phiMin, phiMax;
   mutable Sampler* sampler;
};

#endif
