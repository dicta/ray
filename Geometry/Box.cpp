#include "Box.h"
#include "Parser/Hash.h"

Box::Box() : GeometryObject(), p0(), p1() {
}

void Box::setHash(Hash* hash) {
   p0 = hash->getValue("p0")->getArray();
   p1 = hash->getValue("p1")->getArray();
   setupMaterial(hash->getValue("material")->getHash());
}

bool Box::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   double ox = ray.origin.x;     double oy = ray.origin.y;     double oz = ray.origin.z;
   double dx = ray.direction.x;  double dy = ray.direction.y;  double dz = ray.direction.z;

   double txMin, tyMin, tzMin;
   double txMax, tyMax, tzMax;

   double a = 1.0 / dx;
   if(a >= 0.0) {
      txMin = (p0.x - ox) * a;
      txMax = (p1.x - ox) * a;
   }
   else {
      txMin = (p1.x - ox) * a;
      txMax = (p0.x - ox) * a;
   }

   double b = 1.0 / dy;
   if(b >= 0.0) {
      tyMin = (p0.y - oy) * b;
      tyMax = (p1.y - oy) * b;
   }
   else {
      tyMin = (p1.y - oy) * b;
      tyMax = (p0.y - oy) * b;
   }

   double c = 1.0 / dz;
   if(c >= 0.0) {
      tzMin = (p0.z - oz) * c;
      tzMax = (p1.z - oz) * c;
   }
   else {
      tzMin = (p1.z - oz) * c;
      tzMax = (p0.z - oz) * c;
   }

   int faceIn, faceOut;
   double t0, t1;
   
   // Find largest entering t
   if(txMin > tyMin) {
      t0 = txMin;
      faceIn = (a >= 0.0) ? 0 : 3;
   }
   else {
      t0 = tyMin;
      faceIn = (b >= 0.0) ? 1 : 4;
   }
   if(tzMin > t0) {
      t0 = tzMin;
      faceIn = (c >= 0.0) ? 2 : 5;
   }
   
   // Find smallest exiting t
   if(txMax < tyMax) {
      t1 = txMax;
      faceOut = (a >= 0.0) ? 3 : 0;
   }
   else {
      t1 = tyMax;
      faceOut = (b >= 0.0) ? 4 : 1;
   }
   if(tzMax < t1) {
      t1 = tzMax;
      faceOut = (c >= 0.0) ? 5 : 2;
   }

   if(t0 < t1 && t1 > epsilon) {
      if(t0 > epsilon) {
         tmin = t0;
         getNormal(sr.normal, faceIn);
      }
      else {
         tmin = t1;
         getNormal(sr.normal, faceOut);
      }
      
      sr.localHitPoint = ray.origin + ray.direction * tmin;
      return true;
   }
   return false;
}

bool Box::shadowHit(const Ray& ray, double& tmin) const {
   double ox = ray.origin.x;     double oy = ray.origin.y;     double oz = ray.origin.z;
   double dx = ray.direction.x;  double dy = ray.direction.y;  double dz = ray.direction.z;

   double txMin, tyMin, tzMin;
   double txMax, tyMax, tzMax;

   double a = 1.0 / dx;
   if(a >= 0.0) {
      txMin = (p0.x - ox) * a;
      txMax = (p1.x - ox) * a;
   }
   else {
      txMin = (p1.x - ox) * a;
      txMax = (p0.x - ox) * a;
   }

   double b = 1.0 / dy;
   if(b >= 0.0) {
      tyMin = (p0.y - oy) * b;
      tyMax = (p1.y - oy) * b;
   }
   else {
      tyMin = (p1.y - oy) * b;
      tyMax = (p0.y - oy) * b;
   }

   double c = 1.0 / dz;
   if(c >= 0.0) {
      tzMin = (p0.z - oz) * c;
      tzMax = (p1.z - oz) * c;
   }
   else {
      tzMin = (p1.z - oz) * c;
      tzMax = (p0.z - oz) * c;
   }

   double t0 = max(max(txMin, tyMin), tzMin);
   double t1 = min(min(txMax, tyMax), tzMax);

   if(t0 < t1 && t1 > epsilon) {
      return true;
   }
   return false;
}

void Box::getNormal(Vector3D& n, const int face) const {
   switch(face) {
      case 0: n.set(-1, 0, 0); break;
      case 1: n.set(0, -1, 0); break;
      case 2: n.set(0, 0, -1); break;
      case 3: n.set(1, 0, 0); break;
      case 4: n.set(0, 1, 0); break;
      case 5: n.set(0, 0, 1); break;
   }
}
