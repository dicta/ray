#include "PerfectSpecular.h"

PerfectSpecular::PerfectSpecular() : BRDF(), kr(0.75), color(1.0, 1.0, 1.0) {
}

Color PerfectSpecular::sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi) const {
   float ndotwo = sr.normal.dot(wo);

   // Set wi = the reflection of wo throught the surface normal
   wi = -wo + sr.normal * 2.0 * ndotwo;
   return color * kr / fabs(sr.normal.dot(wi));
}
