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

class Texture;

class Emissive : public Material {
   
public:
   Emissive();
   virtual ~Emissive();

   virtual Color shade(ShadeRecord& sr, const Ray& ray);
   virtual Color areaLightShade(ShadeRecord& sr, const Ray& ray);
   virtual void setHash(Hash* hash);
   virtual Color getLe(const ShadeRecord& sr) const;
   
   virtual void setColor(float r, float g, float b);
   virtual void setDiffuse(float d);
   
   void setTexture(Texture* tex);
   
private:
   float ls;
   Color color;
   Texture* texture;
};

#endif
