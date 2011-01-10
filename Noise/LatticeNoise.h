#ifndef _LATTICE_NOISE_H_
#define _LATTICE_NOISE_H_

#define PERM(x) permutations[(x)& 255]
#define INDEX(ix,iy,iz) PERM((ix)+PERM((iy)+PERM(iz)))

#include "Math/Point3D.h"
#include "Math/Vector3D.h"

class Hash;

class LatticeNoise {

public:
   LatticeNoise(int seed = 253);
   virtual ~LatticeNoise();
   
   void setHash(Hash* h);
   float fractalSum(const Point3D& p) const;
   
   float turbulence(const Point3D& p) const;
   Vector3D vectorTurbulence(const Point3D& p) const;
   
   float fbm(const Point3D& p) const;
   Vector3D vectorFbm(const Point3D& p) const;

   virtual float valueNoise(const Point3D& p) const = 0;
   virtual Vector3D vectorNoise(const Point3D& p) const = 0;
   
protected:
   float values[256];
   Vector3D vectors[256];
   static const unsigned char permutations[256];

private:
   void initValueTable(int seed);
   /** Creates table of random unit vectors distributed among a unit sphere. */
   void initVectorTable(int seed);

   int numOctaves;
   float lacunarity;
   float gain;
   float fsMin, fsMax;
};

#endif
