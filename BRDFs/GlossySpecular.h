#ifndef __GLOSSY_SPECULAR__
#define __GLOSSY_SPECULAR__

#include "BRDF.h"

class Texture;

class GlossySpecular : public BRDF {

public:
   GlossySpecular();
   virtual ~GlossySpecular() {}

   virtual Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const;
   virtual Color rho(const ShadeRecord& sr, const Vector3D& wo) const;
   
   void setKs(const float k) { ks = k; }
   void setExp(const float e) { exp = e; }
//   void setSampler(Sampler* sp, const float exp);
//   void setNormal(const Vector3D& n);

private:
   float ks;
   float exp;
//   Sampler* sampler;
};

#endif
