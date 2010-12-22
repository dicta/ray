#ifndef _LATTICE_NOISE_H_
#define _LATTICE_NOISE_H_

#define PERM(x) permutations[(x)& 255]
#define INDEX(ix,iy,iz) PERM((ix)+PERM((iy)+PERM(iz)))

#include "Math/Point3D.h"

class Hash;

class LatticeNoise {

public:
   LatticeNoise();
   virtual ~LatticeNoise();
   
   void setHash(Hash* h);
   float fractalSum(const Point3D& p) const;
   float turbulence(const Point3D& p) const;
   float fbm(const Point3D& p) const;

protected:
   virtual float valueNoise(const Point3D& p) const = 0;

   float values[256];
   static const unsigned char permutations[256];

private:
   void initValueTable(int seed);

   int numOctaves;
   float lacunarity;
   float gain;
   float fsMin, fsMax;
};

#endif
