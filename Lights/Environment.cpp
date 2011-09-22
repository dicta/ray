#include "Environment.h"
#include "Samplers/Regular.h"
#include "Samplers/MultiJittered.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Materials/Emissive.h"

const float PDF = 1.0 / 2.0 * M_PI;

Environment::Environment() : Light(), material(new Emissive()) {
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
   Vector3D u;

   if(fabs(sr.normal.x) > fabs(sr.normal.y)) {
      u.set(-sr.normal.z, 0.0, sr.normal.x);
   }
   else {
      u.set(0.0, sr.normal.z, -sr.normal.y);
   }
   Vector3D v = sr.normal.cross(u);

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
