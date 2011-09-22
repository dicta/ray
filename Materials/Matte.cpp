#include "Matte.h"
#include "Math/Maths.h"
#include "Lights/LightManager.h"
#include "BRDFs/Lambertian.h"
#include "Textures/Texture.h"
#include "Textures/ImageTexture.h"
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

   ambientBRDF->setKd(hash->getDouble("ka"));
   diffuseBRDF->setKd(hash->getDouble("kd"));

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
}

Color Matte::shade(ShadeRecord& sr, const Ray& ray) {
   Vector3D wo = ray.direction * -1;
   Color L = ambientBRDF->rho(sr, wo) * LightManager::instance().getAmbientLight(sr);

   for(LightIter it = LightManager::instance().begin(); it != LightManager::instance().end(); it++) {
      Color power;
      Vector3D wis;

      for(int s = 0; s < (*it)->getNumLightSamples(); s++) {
         Vector3D wi = (*it)->getLightDirection(sr);
         wis += wi;
         float ndotwi = sr.normal.dot(wi);

         if(ndotwi > 0.0) {
            Ray shadowRay(sr.hitPoint, wi);
            bool inShadow = (*it)->inShadow(shadowRay, sr);

            if(!inShadow) {
               power += (*it)->L(sr) * (*it)->G(sr) * ndotwi / (*it)->pdf(sr);
            }
         }
      }

      power = power / (*it)->getNumLightSamples();
      wis.normalize();
      L += diffuseBRDF->f(sr, wo, wis) * power;
   }

   return L;
}

float Matte::getAlpha(const ShadeRecord& sr, const Ray& ray) const {
   return diffuseBRDF->getAlpha(sr);
}

void Matte::setColor(float r, float g, float b) {
   ambientBRDF->setColor(new Color(r, g, b));
   diffuseBRDF->setColor(new Color(r, g, b));
}

void Matte::setDiffuse(float d) {
   ambientBRDF->setKd(d);
   diffuseBRDF->setKd(d);
}

void Matte::setTexture(string texture) {
   ImageTexture* tex = new ImageTexture();
   tex->setTextureFile(texture);
   ambientBRDF->setTexture(tex);

   tex = new ImageTexture();
   tex->setTextureFile(texture);
   diffuseBRDF->setTexture(tex);
}

void Matte::setAmbientColor(Color* c) {
   ambientBRDF->setColor(c);
   ambientBRDF->setKd(0.2);
}

void Matte::setDiffuseColor(Color* c) {
   diffuseBRDF->setColor(c);
   diffuseBRDF->setKd(0.8);
}
