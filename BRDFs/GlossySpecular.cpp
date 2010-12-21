#include "GlossySpecular.h"
#include "Samplers/MultiJittered.h"
#include "Textures/Texture.h"
#include <math.h>

GlossySpecular::GlossySpecular() : ks(0.0), exp(0) { //, sampler(NULL) {
}

//void GlossySpecular::setSampler(Sampler* sp, const float exp) {
//	sampler = sp;
//   // map to hemispher
//}

Color GlossySpecular::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   Color L;
	float ndotwi = sr.normal.dot(wi);
	Vector3D r(wi * -1.0 + sr.normal * 2.0 * ndotwi);
	float rdotwo = r.dot(wo); 		
		
   if (rdotwo > 0.0) {
      L += ks * pow(rdotwo, exp);
   }
			
	return L;
}

Color GlossySpecular::rho(const ShadeRecord& sr, const Vector3D& wo) const {
   return BLACK;
}
