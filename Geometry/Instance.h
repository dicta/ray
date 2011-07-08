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
   Instance(GeometryObject* obj);
   virtual ~Instance();

   void setPosition(const Point3D& position);
   void translate(double x, double y, double z);
   void scale(double x, double y, double z);
   void rotateX(double angle);
   void rotateY(double angle);
   void rotateZ(double angle);
   void computeBBox();
   void reset();

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

inline void Instance::setPosition(const Point3D& position) {
   fwdMatrix.setPosition(position);
   invMatrix.setPosition(-position);
}

inline void Instance::translate(double x, double y, double z) {
   fwdMatrix.translate(x, y, z);
   invMatrix.invTranslate(x, y, z);
}

inline void Instance::scale(double x, double y, double z) {
   fwdMatrix.scale(x, y, z);
   invMatrix.invScale(x, y, z);
}

inline void Instance::rotateX(double angle) {
   fwdMatrix.rotateX(angle);
   invMatrix.invRotateX(angle);
}

inline void Instance::rotateY(double angle) {
   fwdMatrix.rotateY(angle);
   invMatrix.invRotateY(angle);
}

inline void Instance::rotateZ(double angle) {
   fwdMatrix.rotateZ(angle);
   invMatrix.invRotateZ(angle);
}

inline void Instance::reset() {
   fwdMatrix.setIdentity();
   invMatrix.setIdentity();
}

#endif
