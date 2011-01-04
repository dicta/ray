#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "GeometryObject.h"
#include "Math/Matrix.h"

/**
 * Possible transformations:
 *
 * <ul>
 * <li>translate [ x y z ]
 * <li>scale [ x y z ]
 * <li>rotateX angle
 * <li>rotateY angle
 * <li>rotateZ angle
 * </ul>
 */
class Instance : public GeometryObject {
   
public:
   Instance();
   virtual ~Instance();

   /**
    * Hash values:
    * <ul>
    * <li>object - The object being transformed. This is an style hash with the key object set to the object type.
    * <li>transforms - Array of transformations
    * </ul>
    */
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
private:
   GeometryObject* object;
   Matrix invMatrix;
   Matrix fwdMatrix;
};

#endif
