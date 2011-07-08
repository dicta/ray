#include "Environment.h"
#include "Samplers/Regular.h"
#include "Samplers/MultiJittered.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Materials/Emissive.h"

const Vector3D up(0, 1, 0);
const Vector3D up2(0.0034, 1.0, 0.0071);
const float PDF = 1.0 / 2.0 * M_PI;

Environment::Environment() : Light(), material(new Emissive()), numLightSamples(1) {
}

Environment::~Environment() {
   delete material;
}

void Environment::setHash(Hash* hash) {
   material->setHash(hash);

   int numSamples = hash->getInteger("numSamples");
   if(numSamples == 1) {
      sampler = new Regular(numSamples);
   }
   else {
      sampler = new MultiJittered(numSamples);
   }
   sampler->mapSamplesToHemisphere(1);

   if(hash->contains("numLightSamples")) {
      numLightSamples = hash->getInteger("numLightSamples");
   }
}

Vector3D Environment::getLightDirection(ShadeRecord& sr) {
   Vector3D v;
   if(sr.normal == up) {
      v = up2.cross(sr.normal);
   }
   else {
      v = up.cross(sr.normal);
   }

   v.normalize();
   Vector3D u = v.cross(sr.normal);
   Point3D* sp = sampler->sampleHemisphere();
   return u * sp->x + v * sp->y + sr.normal * sp->z;
}

bool Environment::inShadow(const Ray& ray, const ShadeRecord& sr) {
   double t;

   if(GeometryManager::instance().getGrid().shadowHit(ray, t)) {
      return true;
   }
/*
   for(GeometryIter it = GeometryManager::instance().begin(); it != GeometryManager::instance().end(); it++) {
      if((*it)->shadowHit(ray, t)) {
         return true;
      }
   }
*/
   return false;
}

Color Environment::L(const ShadeRecord& sr) {
   return material->getLe(sr);
}

float Environment::pdf(const ShadeRecord& sr) {
   return PDF;
}
