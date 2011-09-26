#include "Compound.h"
#include <math.h>


Compound::Compound() : GeometryObject() {
}

Compound::~Compound() {
   while (!objs.empty()) {
     GeometryObject* o = *objs.begin();
     objs.pop_front();
     delete o;
   }
}

void Compound::setHash(Hash* hash) {
   for(CompoundIter it = objs.begin(); it != objs.end(); it++) {
      (*it)->setHash(hash);
   }
}

void Compound::addObject(GeometryObject* obj) {
   objs.push_back(obj);
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

   for(CompoundIter it = objs.begin(); it != objs.end(); it++) {
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
      sr.material = mat;
   }

   return hit;
}

bool Compound::shadowHit(const Ray& ray, double& tmin) const {
   if(!bbox.hit(ray)) {
      return false;
   }

   tmin = 1.7 * pow(10.0, 308.0);

   for(CompoundIter it = objs.begin(); it != objs.end(); it++) {
      if((*it)->shadowHit(ray, tmin)) {
         return true;
      }
   }

   return false;
}
