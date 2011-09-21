#include "Compound.h"
#include <math.h>

typedef vector<GeometryObject*>::const_iterator CompoundIter;

Compound::Compound() : GeometryObject() {
}

Compound::~Compound() {
   for(unsigned int i = 0; i < objects.size(); i++) {
      delete objects[i];
   }
   objects.clear();
}

void Compound::setHash(Hash* hash) {
   for(CompoundIter it = objects.begin(); it != objects.end(); it++) {
      (*it)->setHash(hash);
   }
}

void Compound::addObject(GeometryObject* obj) {
   objects.push_back(obj);
   bbox.expand(obj->bbox);
}

bool Compound::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   if(!bbox.hit(ray)) {
      return false;
   }

   double t = 0;
   tmin = 1.7 * pow(10.0, 308.0);
   bool hit = false;
   Vector3D normal;
   Point3D localHitPoint;
   shared_ptr<Material> mat;
   double tu, tv;
   Vector3D dpdu;
   Vector3D dpdv;

   for(CompoundIter it = objects.begin(); it != objects.end(); it++) {
      if((*it)->hit(ray, t, sr) && (t < tmin)) {
         hit = true;
         tmin = t;
         normal = sr.normal;
         localHitPoint = sr.localHitPoint;
         mat = (*it)->getMaterial();
         tu = sr.tu;
         tv = sr.tv;
         dpdu = sr.dpdu;
         dpdv = sr.dpdv;
      }
   }

   if(hit) {
      sr.normal = normal;
      sr.localHitPoint = localHitPoint;
      sr.tu = tu;
      sr.tv = tv;
      sr.dpdu = dpdu;
      sr.dpdv = dpdv;
      material = mat;
   }

   return hit;
}

bool Compound::shadowHit(const Ray& ray, double& tmin) const {
   if(!bbox.hit(ray)) {
      return false;
   }

   tmin = 1.7 * pow(10.0, 308.0);

   for(CompoundIter it = objects.begin(); it != objects.end(); it++) {
      if((*it)->shadowHit(ray, tmin)) {
         return true;
      }
   }

   return false;
}
