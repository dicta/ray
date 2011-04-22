#ifndef _ANNULUS_H_
#define _ANNULUS_H_

#include "GeometryObject.h"

/**
 * Creates an annulus (disk with a hollow center).
 *
 * Hash values:
 * <ul>
 * <li>center [x y z] - Center of the disk.
 * <li>normal [x y z] - Normal of the disk.
 * <li>innerRadius <i>value</i> - Radius of the the center opening. Double.
 * <li>outerRadius <i>value</i> - Radius of the annulus. Double.
 * <li>angleRange [min max] - Optional angle range for a part annulus. Doubles.
 * </ul>
 */
class Annulus : public GeometryObject {

public:
   Annulus();
   Annulus(const Point3D& c, const Vector3D& n, double inner, double outer);
   virtual ~Annulus() {}
   
   void setAngleRange(double min, double max);
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
private:
   bool partCheck(const Point3D& hit) const;

   Point3D center;
   Vector3D normal;
   double innerSquared;
   double outerSquared;
   
   bool angleRange;
   double minAngle;
   double maxAngle;
};

#endif
