#include "PerfectSpecular.h"

PerfectSpecular::PerfectSpecular() : BRDF(), kr(0.0), color() {
}

Color PerfectSpecular::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
	return BLACK;
}

Color PerfectSpecular::rho(const ShadeRecord& sr, const Vector3D& wo) const {
   return BLACK;
}
