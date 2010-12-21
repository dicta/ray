/*
 *  Emissive.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/20/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _EMISSIVE_H_
#define _EMISSIVE_H_

#include "Material.h"

class Emissive : public Material {
   
public:
   virtual Color shade(const ShadeRecord& sr, const Ray& ray);
   virtual Color areaLightShade(const ShadeRecord& sr, const Ray& ray);
   virtual void setHash(Hash* hash);
   virtual Color getLe(const ShadeRecord& sr) const;
   
private:
   float ls;
   Color color;
};

#endif
