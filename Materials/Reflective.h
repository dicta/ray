#ifndef _REFLECTIVE_H_
#define _REFLECTIVE_H_

#include "Phong.h"

class PerfectSpecular;
class Hash;

class Reflective : public Phong {
   
public:
   Reflective();
   virtual ~Reflective();

   virtual Color shade(ShadeRecord& sr, const Ray& ray);
   virtual Color areaLightShade(ShadeRecord& sr, const Ray& ray);
   virtual void setHash(Hash* hash);
   
private:
   PerfectSpecular* psBRDF;
};

#endif
