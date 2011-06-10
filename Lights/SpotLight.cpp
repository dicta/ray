#include "SpotLight.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Math/Maths.h"

SpotLight::SpotLight() : Light(), ls(1.0), color(1, 1, 1), location(), direction(), cosWidth(0), cosFalloff(0) {
}

Vector3D SpotLight::getLightDirection(ShadeRecord& sr) {
   return -direction;
}

bool SpotLight::inShadow(const Ray& ray, const ShadeRecord& sr) {
   double t;
   double d = location.distance(ray.origin);

   if(GeometryManager::instance().getGrid().shadowHit(ray, t) && (t < d)) {
      return true;
   }
/*
   for(GeometryIter it = GeometryManager::instance().begin(); it != GeometryManager::instance().end(); it++) {
      if((*it)->shadowHit(ray, t) && (t < d)) {
         return true;
      }
   }
*/
   return false;
}

void SpotLight::setHash(Hash* hash) {
   location.set(hash->getValue("location")->getArray());

   direction.set(hash->getValue("direction")->getArray());
   direction.normalize();

   color.set(hash->getValue("color")->getArray());
   ls = hash->getDouble("radiance");
   
   float width = hash->getDouble("width");
   cosWidth = cos(DEG_TO_RAD * width);
   
   float falloff = hash->getDouble("falloff");
   cosFalloff = cos(DEG_TO_RAD * falloff);
}

Color SpotLight::L(const ShadeRecord& sr) {
   double f = falloff(sr);
   return color * ls * f;
}

double SpotLight::falloff(const ShadeRecord& sr) const {
   Vector3D v = sr.localHitPoint - location;
   v.normalize();
   double cosTheta = v.dot(direction);

   if(cosTheta < cosWidth) {
      return 0;
   }
   if(cosTheta > cosFalloff) {
      return 1;
   }
   double delta = (cosTheta - cosWidth) / (cosFalloff - cosWidth);
   return delta * delta * delta * delta;
}
