#include "CubicNoise.h"
#include "Math/Maths.h"
#include <math.h>

CubicNoise::CubicNoise(int seed) : LatticeNoise(seed){
}

CubicNoise::~CubicNoise() {
}

float CubicNoise::valueNoise(const Point3D& p) const {
   float xknots[4], yknots[4], zknots[4];

   int ix = (int) floor(p.x);
   float fx = p.x - ix;
   
   int iy = (int) floor(p.y);
   float fy = p.y - iy;
   
   int iz = (int) floor(p.z);
   float fz = p.z - iz;
   
   for(int k = -1; k <= 2; k++) {
      for(int j = -1; j <= 2; j++) {
         for(int i = -1; i <= 2; i++) {
            xknots[i+1] = values[INDEX(ix + i, iy+ j, iz + k)];
         }
         yknots[j+1] = spline(fx, xknots);
      }
      zknots[k+1] = spline(fy, yknots);
   }

   return clamp(spline(fz, zknots), -1.0, 1.0);
}

Vector3D CubicNoise::vectorNoise(const Point3D& p) const {
   Vector3D xknots[4], yknots[4], zknots[4];

   int ix = (int) floor(p.x);
   float fx = p.x - ix;
   
   int iy = (int) floor(p.y);
   float fy = p.y - iy;

   int iz = (int) floor(p.z);
   float fz = p.z - iz;

   for(int k = -1; k <= 2; k++) {
      for(int j = -1; j <= 2; j++) {
         for(int i = -1; i <=2; i++) {
            xknots[i+1] = vectors[INDEX(ix + i, iy + j, iz + k)];
         }
         yknots[j+1] = spline(fx, xknots);
      }
      zknots[k+1] = spline(fy, yknots);
   }
   return spline(fz, zknots);
}
