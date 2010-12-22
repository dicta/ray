#include "CubicNoise.h"
#include "Math/Maths.h"
#include <math.h>

CubicNoise::CubicNoise() {
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

float CubicNoise::spline(const float x, const float knots[]) const {
   float c3 = knots[0] * -0.5 + knots[1] * 1.5 - knots[2] * 1.5 + knots[3] * 0.5;
   float c2 = knots[0] - knots[1] * 2.5 + knots[2] * 2.0 - knots[3] * 0.5;
   float c1 = (knots[2] - knots[0]) * 0.5;
   float c0 = knots[1];

   return ((c3 * x + c2) * x + c1) * x + c0;
}
