#include "Lambertian.h"
#include "Math/Maths.h"
#include "Textures/Texture.h"

Lambertian::Lambertian() : BRDF(), kd(0.0), color(NULL), texture(NULL) {
}

Lambertian::~Lambertian() {
   if(color != NULL) {
      delete color;
   }
   if(texture != NULL) {
      delete texture;
   }
}

Color Lambertian::f(const ShadeRecord& sr, const Vector3D& wo, const Vector3D& wi) const {
   if(texture != NULL) {
      Color tc = texture->getColor(sr);
      if(color != NULL) {
         tc = tc * (*color);
      }
      return tc * kd * INV_PI;
   }
   if(color != NULL) {
      return (*color * kd * INV_PI);
   }
   return BLACK;
}

Color Lambertian::rho(const ShadeRecord& sr, const Vector3D& wo) const {
   if(texture != NULL) {
      return texture->getColor(sr) * kd;
   }
   if(color != NULL) {
      return (*color * kd);
   }
   return BLACK;
}

float Lambertian::getAlpha(const ShadeRecord& sr) const {
   if(texture != NULL) {
      return texture->getAlpha(sr);
   }
   return color->alpha;
}
