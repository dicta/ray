/*
 *  Phong.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/19/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Phong.h"
#include "Math/Maths.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"
#include "BRDFs/Lambertian.h"
#include "BRDFs/GlossySpecular.h"
#include "Textures/Texture.h"
#include "Parser/Hash.h"

Phong::Phong() : ambientBRDF(new Lambertian()), diffuseBRDF(new Lambertian()), specularBRDF(new GlossySpecular()) {
}

Phong::~Phong() {
   delete ambientBRDF;
   delete diffuseBRDF;
   delete specularBRDF;
}

void Phong::setHash(Hash* hash) {
   bool colorSet = false;
   bool textureSet = false;
   
   if(hash->contains("texture")) {
      textureSet = true;
      
      ambientBRDF->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
      diffuseBRDF->setTexture(Texture::createTexture(hash->getValue("texture")->getHash()));
   }
   else if(hash->contains("color")) {
      colorSet = true;
      
      Array* a = hash->getValue("color")->getArray();
      ambientBRDF->setColor(new Color(a));
      diffuseBRDF->setColor(new Color(a));
   }
   
   if(!colorSet && !textureSet) {
      ambientBRDF->setColor(new Color(BLACK));
      diffuseBRDF->setColor(new Color(BLACK));
   }
   
   ambientBRDF->setKd(hash->getDouble("ka"));
   diffuseBRDF->setKd(hash->getDouble("kd"));
   specularBRDF->setKs(hash->getDouble("ks"));
   specularBRDF->setExp(hash->getDouble("exp"));
}
    
Color Phong::shade(ShadeRecord& sr, const Ray& ray) {
   Vector3D wo = ray.direction * -1;
   Color L = ambientBRDF->rho(sr, wo) * LightManager::instance().getAmbientLight(sr);
   
   for(LightIter it = LightManager::instance().begin(); it != LightManager::instance().end(); it++) {
      Vector3D wi = (*it)->getLightDirection(sr);
      float ndotwi = sr.normal.dot(wi);
      
      if(ndotwi > 0.0) {
         Ray shadowRay(sr.hitPoint, wi);
         bool inShadow = (*it)->inShadow(shadowRay, sr);
         
         if(!inShadow) {
            L += (diffuseBRDF->f(sr, wo, wi) + specularBRDF->f(sr, wo, wi)) * (*it)->L(sr) * ndotwi;
         }
      }
   }
   
   return L;
}

Color Phong::areaLightShade(ShadeRecord& sr, const Ray& ray) {
   return shade(sr, ray);
}

void Phong::setColor(float r, float g, float b) {
   ambientBRDF->setColor(new Color(r, g, b));
   diffuseBRDF->setColor(new Color(r, g, b));
}

void Phong::setDiffuse(float d) {
   diffuseBRDF->setKd(d);
}

void Phong::setAmbientColor(Color* c) {
   ambientBRDF->setColor(c);
   ambientBRDF->setKd(0.2);
}

void Phong::setDiffuseColor(Color* c) {
   diffuseBRDF->setColor(c);
   diffuseBRDF->setKd(0.6);
}

void Phong::setSpecularColor(Color* c) {
   specularBRDF->setColor(c);
}

void Phong::setSpecularHighlight(float sh) {
   specularBRDF->setExp(sh);
}

void Phong::setSpecularPercent(float p) {
   specularBRDF->setKs(p);
}
