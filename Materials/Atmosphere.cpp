#include "Atmosphere.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"
#include "Math/Maths.h"
#include "Parser/Hash.h"
#include "Math/Matrix.h"

void Atmosphere::setHash(Hash* hash) {
   color.set(hash->getValue("color")->getArray());
}

Color Atmosphere::shade(ShadeRecord& sr, const Ray& ray) {
   return Color(color.red, color.green, color.blue, getAlpha(sr, ray));
}

float Atmosphere::getAlpha(const ShadeRecord& sr, const Ray& ray) const {
   Vector3D normal = ray.calculateNormalMatrix() * sr.normal;
   normal.normalize();

   double dP = 1.0 - fabs(-ray.direction.dot(normal));
   double alpha = 1.0 - pow(dP, 5.0);
   alpha *= dP;

   LightIter it = LightManager::instance().begin();
   ShadeRecord& sr2 = const_cast<ShadeRecord&>(sr);
   Vector3D lightDir = (*it)->getLightDirection(sr2);
   alpha = clamp(pow(alpha, 4.0) * sr.normal.dot(lightDir) * 7.0, 0.0, 1.0);
   
   return alpha;
}
