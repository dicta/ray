#include "TInstance.h"
#include "Parser/Hash.h"

TInstance::TInstance() : texture(NULL), invMatrix(){
}

TInstance::TInstance(Texture* tex) : texture(tex), invMatrix() {
}

TInstance::~TInstance() {
}

void TInstance::setHash(Hash* hash) {
   texture = Texture::createTexture(hash->getValue("texture")->getHash());

   if(hash->contains("transforms")) {
      Array* transforms = hash->getValue("transforms")->getArray();
      int idx = 0;
      while(idx < transforms->size()) {
         string type = transforms->at(idx)->getString();
         idx++;

         if(type == "translate") {
            Array* a = transforms->at(idx)->getArray();
            invMatrix.invTranslate(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
         }
         else if(type == "scale") {
            Array* a = transforms->at(idx)->getArray();
            invMatrix.invScale(a->at(0)->getDouble(), a->at(1)->getDouble(), a->at(2)->getDouble());
         }
         else if(type == "rotateX") {
            invMatrix.invRotateX(transforms->at(idx)->getDouble());
         }
         else if(type == "rotateY") {
            invMatrix.invRotateY(transforms->at(idx)->getDouble());
         }
         else if(type == "rotateZ") {
            invMatrix.invRotateZ(transforms->at(idx)->getDouble());
         }
         idx++;
      }
   }
}

Color TInstance::getColor(const ShadeRecord& sr) const {
   ShadeRecord lsr(sr);
   lsr.localHitPoint = invMatrix * sr.localHitPoint;
   return texture->getColor(lsr);
}

float TInstance::getAlpha(const ShadeRecord& sr) const {
   ShadeRecord lsr(sr);
   lsr.localHitPoint = invMatrix * sr.localHitPoint;
   return texture->getAlpha(lsr);
}
