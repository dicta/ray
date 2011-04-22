#ifndef __BRDF__
#define __BRDF__

#include "Utility/Color.h"
#include "Math/Vector3D.h"
#include "Math/Point3D.h"
#include "Utility/ShadeRecord.h"

class Sampler;

class BRDF {

public:
   BRDF();
   virtual ~BRDF();

   void setSampler(Sampler* sampler);
   virtual Color f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const;
   virtual Color rho(const ShadeRecord& sr, const Vector3D& wo) const;
   virtual float getAlpha(const Point3D& p) const { return 1.0; }

protected:
   Sampler* sampler;
};

#endif
