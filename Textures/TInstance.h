#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "Texture.h"
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
class TInstance : public Texture {

public:
   TInstance();
   TInstance(Texture* texture);
   virtual ~TInstance();

   void translate(double x, double y, double z);
   void scale(double x, double y, double z);
   void rotateX(double angle);
   void rotateY(double angle);
   void rotateZ(double angle);

   /**
    * Hash values:
    * <ul>
    * <li>texture - The object being transformed. This is an style hash with the key object set to the object type.
    * <li>transforms - Array of transformations
    * </ul>
    */
   virtual void setHash(Hash* hash);
   virtual Color getColor(const ShadeRecord& sr) const;
   virtual float getAlpha(const Point3D& p) const { return texture->getAlpha(p); }

private:
   Texture* texture;
   Matrix invMatrix;
};

inline void TInstance::translate(double x, double y, double z) {
   invMatrix.invTranslate(x, y, z);
}

inline void TInstance::scale(double x, double y, double z) {
   invMatrix.invScale(x, y, z);
}

inline void TInstance::rotateX(double angle) {
   invMatrix.invRotateX(angle);
}

inline void TInstance::rotateY(double angle) {
   invMatrix.invRotateY(angle);
}

inline void TInstance::rotateZ(double angle) {
   invMatrix.invRotateZ(angle);
}

#endif
