#include "Matte.h"
#include "Math/Maths.h"
#include "Lights/LightManager.h"
#include "BRDFs/Lambertian.h"
#include "Textures/Texture.h"
#include "Parser/Hash.h"
#include "Lights/Light.h"

Matte::Matte() : ambientBRDF(new Lambertian()), diffuseBRDF(new Lambertian()) {
}

Matte::~Matte() {
   delete ambientBRDF;
   delete diffuseBRDF;
}

void Matte::setHash(Hash* hash) {
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
}

Color Matte::shade(ShadeRecord& sr, const Ray& ray) {
   Vector3D wo = ray.direction * -1;
   Color L = ambientBRDF->rho(sr, wo) * LightManager::instance().getAmbientLight(sr);
   
   for(LightIter it = LightManager::instance().begin(); it != LightManager::instance().end(); it++) {
      Vector3D wi = (*it)->getLightDirection(sr);
      float ndotwi = sr.normal.dot(wi);
      
      if(ndotwi > 0.0) {
         Ray shadowRay(sr.hitPoint, wi);
         bool inShadow = (*it)->inShadow(shadowRay, sr);
         
         if(!inShadow) {
            L += diffuseBRDF->f(sr, wo, wi) * (*it)->L(sr) * ndotwi;
         }
      }
   }
   
   return L;
}

Color Matte::areaLightShade(ShadeRecord& sr, const Ray& ray) {
   Vector3D wo = ray.direction * -1;
   Color L = ambientBRDF->rho(sr, wo) * LightManager::instance().getAmbientLight(sr);
   
   for(LightIter it = LightManager::instance().begin(); it != LightManager::instance().end(); it++) {
      Vector3D wi = (*it)->getLightDirection(sr);
      float ndotwi = sr.normal.dot(wi);
      
      if(ndotwi > 0.0) {
         Ray shadowRay(sr.hitPoint, wi);
         bool inShadow = (*it)->inShadow(shadowRay, sr);
         
         if(!inShadow) {
            L += diffuseBRDF->f(sr, wo, wi) * (*it)->L(sr) * (*it)->G(sr) * ndotwi / (*it)->pdf(sr);
         }
      }
   }
   
   return L;
}
