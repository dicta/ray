#ifndef _CUBIC_NOISE_H_
#define _CUBIC_NOISE_H_

#include "LatticeNoise.h"

class CubicNoise : public LatticeNoise {

public:
   CubicNoise();
   virtual ~CubicNoise();
   
protected:
   virtual float valueNoise(const Point3D& p) const;
   
private:
   float spline(const float x, const float knots[]) const;
};

#endif
