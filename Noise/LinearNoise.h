#ifndef _LINEAR_NOISE_H_
#define _LINEAR_NOISE_H_

#include "LatticeNoise.h"

class LinearNoise : public LatticeNoise {

public:
   LinearNoise();
   virtual ~LinearNoise();

protected:
   virtual float valueNoise(const Point3D& p) const;
   virtual Vector3D vectorNoise(const Point3D& p) const;
};

#endif
