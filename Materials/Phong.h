/*
 *  Phong.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PHONG_H_
#define _PHONG_H_

#include "Material.h"

class Lambertian;
class GlossySpecular;
class Texture;

class Phong : public Material {
   
public:
   Phong();
   ~Phong();
   
   virtual Color shade(ShadeRecord& sr, const Ray& ray);
   virtual Color areaLightShade(ShadeRecord& sr, const Ray& ray);
   virtual void setHash(Hash* hash);
   
   virtual void setColor(float r, float g, float b);
   virtual void setDiffuse(float d);
   void setTexture(string texture);
   
   void setAmbientColor(Color* c);
   void setDiffuseColor(Color* c);
   void setSpecularColor(Color* c);
   void setSpecularHighlight(float sh);
   void setSpecularPercent(float p);

private:
   Lambertian* ambientBRDF;
   Lambertian* diffuseBRDF;
   GlossySpecular* specularBRDF;
};

#endif
