/*
 *  Material.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Ray.h"

class Hash;

class Material {
   
public:
   virtual Color shade(ShadeRecord& sr, const Ray& ray) { return BLACK; }
   virtual Color areaLightShade(ShadeRecord& sr, const Ray& ray) { return BLACK; }
   virtual void setHash(Hash* hash) = 0;
   virtual Color getLe(const ShadeRecord& sr) const { return BLACK; }
};

#endif
