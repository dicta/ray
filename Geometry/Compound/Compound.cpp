#include "Compound.h"
#include <math.h>

Compound::Compound() : GeometryObject(), bbox() {
}

Compound::~Compound() {
   for(unsigned int i = 0; i < objects.size(); i++) {
      delete objects[i];
   }
   objects.clear();
}

bool Compound::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   if(!bbox.hit(ray)) {
      return false;
   }
   
   double t;
   tmin = 1.7 * pow(10.0, 308.0);
   bool hit = false;
   Vector3D normal;
   Point3D localHitPoint;
   
   for(GeometryIter it = objects.begin(); it != objects.end(); it++) {
      if((*it)->hit(ray, t, sr) && (t < tmin)) {
         hit = true;
         tmin = t;
         normal = sr.normal;
         localHitPoint = sr.localHitPoint;
      }
   }
   
   if(hit) {
      sr.t = tmin;
      sr.normal = normal;
      sr.localHitPoint = localHitPoint;
   }

   return hit;
}

bool Compound::shadowHit(const Ray& ray, double& tmin) const {
   if(!bbox.hit(ray)) {
      return false;
   }
   
   tmin = 1.7 * pow(10.0, 308.0);
   
   for(GeometryIter it = objects.begin(); it != objects.end(); it++) {
      if((*it)->shadowHit(ray, tmin)) {
         return true;
      }
   }

   return false;
}
