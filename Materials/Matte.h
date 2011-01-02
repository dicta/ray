/*
 *  Matte.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MATTE_H_
#define _MATTE_H_

#include "Material.h"

class Lambertian;
class Texture;

class Matte : public Material {
   
public:
   Matte();
   ~Matte();

   virtual Color shade(ShadeRecord& sr, const Ray& ray);
   virtual Color areaLightShade(ShadeRecord& sr, const Ray& ray);
   virtual void setHash(Hash* hash);

private:
   Lambertian* ambientBRDF;
   Lambertian* diffuseBRDF;
};

#endif
