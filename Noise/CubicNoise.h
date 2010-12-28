#ifndef _CUBIC_NOISE_H_
#define _CUBIC_NOISE_H_

#include "LatticeNoise.h"

class CubicNoise : public LatticeNoise {

public:
   CubicNoise(int seed = 253);
   virtual ~CubicNoise();
   
protected:
   virtual float valueNoise(const Point3D& p) const;
   virtual Vector3D vectorNoise(const Point3D& p) const;
};

template<class T>
T spline(const float x, const T knots[]) {
   T c3 = knots[0] * -0.5 + knots[1] * 1.5 - knots[2] * 1.5 + knots[3] * 0.5;
   T c2 = knots[0] - knots[1] * 2.5 + knots[2] * 2.0 - knots[3] * 0.5;
   T c1 = (knots[2] - knots[0]) * 0.5;
   T c0 = knots[1];

   return ((c3 * x + c2) * x + c1) * x + c0;
}

#endif
