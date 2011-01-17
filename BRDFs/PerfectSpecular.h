#ifndef __PERFECT_SPECULAR__
#define __PERFECT_SPECULAR__

#include "BRDF.h"
#include "Math/Vector3D.h"

class PerfectSpecular: public BRDF 
{

public:
   PerfectSpecular();
   ~PerfectSpecular() {}

   Color sample_f(const ShadeRecord& sr, const Vector3D& wo, Vector3D& wi) const;
   
   void setKr(const float k) { kr = k; }
   void setColor(const Color& c) { color = c; }
   
private:
   float kr;
   Color color;
};

#endif
