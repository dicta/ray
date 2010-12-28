#include "LinearNoise.h"
#include "Math/Maths.h"
#include <math.h>

#define d(i, j, k) values[INDEX(i, j, k)]
#define vecd(i, j, k) vectors[INDEX(i, j, k)]

LinearNoise::LinearNoise() {
}

LinearNoise::~LinearNoise() {
}

float LinearNoise::valueNoise(const Point3D& p) const {
   int ix = (int) floor(p.x);
   float fx = p.x - ix;
   
   int iy = (int) floor(p.y);
   float fy = p.y - iy;
   
   int iz = (int) floor(p.z);
   float fz = p.z - iz;
   
   // Interpolate 4 x values between all 8 corners
   float x0 = lerp(fx, d(ix, iy, iz), d(ix, iy, iz+1));
   float x1 = lerp(fx, d(ix, iy+1, iz), d(ix, iy+1, iz+1));
   float x2 = lerp(fx, d(ix+1, iy, iz), d(ix+1, iy, iz+1));
   float x3 = lerp(fx, d(ix+1, iy+1, iz), d(ix+1, iy+1, iz+1));
   
   // Interpolate 2 y values between interpolated x values
   float y0 = lerp(fy, x0, x1);
   float y1 = lerp(fy, x2, x3);
   
   // Interpolate final z value between interpolated y values
   return lerp(fz, y0, y1);
}

Vector3D LinearNoise::vectorNoise(const Point3D& p) const {
   int ix = (int) floor(p.x);
   float fx = p.x - ix;

   int iy = (int) floor(p.y);
   float fy = p.y - iy;

   int iz = (int) floor(p.z);
   float fz = p.z - iz;

   // Interpolate 4 x values between all 8 corners
   Vector3D x0 = lerp(fx, vecd(ix, iy, iz), vecd(ix, iy, iz+1));
   Vector3D x1 = lerp(fx, vecd(ix, iy+1, iz), vecd(ix, iy+1, iz+1));
   Vector3D x2 = lerp(fx, vecd(ix+1, iy, iz), vecd(ix+1, iy, iz+1));
   Vector3D x3 = lerp(fx, vecd(ix+1, iy+1, iz), vecd(ix+1, iy+1, iz+1));
   
   // Interpolate 2 y values between interpolated x values
   Vector3D y0 = lerp(fy, x0, x1);
   Vector3D y1 = lerp(fy, x2, x3);

   // Interpolate final z value between interpolated y values
   return lerp(fz, y0, y1);
}
