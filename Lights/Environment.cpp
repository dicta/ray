#include "Environment.h"
#include "Samplers/Regular.h"
#include "Samplers/MultiJittered.h"
#include "Parser/Hash.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Materials/Emissive.h"

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
}

Vector3D Environment::getLightDirection(ShadeRecord& sr) {
   w = sr.normal;
   v = Vector3D(0.0034, 1.0, 0.0071).cross(w);
   v.normalize();
   u = v.cross(w);
   Point3D* sp = sampler->sampleHemisphere();
   wi = u * sp->x + v * sp->y + w * sp->z;
   
   return wi;
}

bool Environment::inShadow(const Ray& ray, const ShadeRecord& sr) {
   double t;

   for(GeometryIter it = GeometryManager::instance().begin(); it != GeometryManager::instance().end(); it++) {
      if((*it)->shadowHit(ray, t)) {
         return true;
      }
   }

   return false;
}

Color Environment::L(const ShadeRecord& sr) {
   return material->getLe(sr);
}
