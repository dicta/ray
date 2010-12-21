#include "BRDF.h"
#include "Samplers/Sampler.h"

BRDF::BRDF() : sampler(NULL) {
}

BRDF::~BRDF() {
   if(sampler) {
      delete sampler;
   }
}

void BRDF::setSampler(Sampler* s) {
   sampler = s;
   // map to hemispher
}

Color BRDF::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   return BLACK;
}
	
Color BRDF::rho(const ShadeRecord& sr, const Vector3D& wo) const {
   return BLACK;
}
