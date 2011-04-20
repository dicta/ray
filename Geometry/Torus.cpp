#include "Torus.h"
#include "Math/Maths.h"
#include "Parser/Hash.h"
#include <math.h>

Torus::Torus() :
   GeometryObject(),
   a(0),
   b(0),
   bbox(),
   thetaRange(false),
   phiRange(false),
   minTheta(0),
   maxTheta(0),
   minPhi(0),
   maxPhi(0)
{
}

Torus::Torus(double _a, double _b) :
   GeometryObject(),
   a(_a),
   b(_b),
   bbox(),
   thetaRange(false),
   phiRange(false),
   minTheta(0),
   maxTheta(0),
   minPhi(0),
   maxPhi(0)
{
   bbox.x0 = bbox.z0 = -a - b;
   bbox.y0 = -b;
   bbox.x1 = bbox.z1 = a + b;
   bbox.y1 = b;
}

void Torus::setHash(Hash* hash) {
   a = hash->getDouble("a");
   b = hash->getDouble("b");
   setupMaterial(hash->getValue("material")->getHash());
   
   if(hash->contains("thetaRange")) {
      thetaRange = true;
      Array* a = hash->getValue("thetaRange")->getArray();
      minTheta = a->at(0)->getDouble() * DEG_TO_RAD;
      cosThetaMin = cos(minTheta);
      maxTheta = a->at(1)->getDouble() * DEG_TO_RAD;
      cosThetaMax = cos(maxTheta);
   }
   
   if(hash->contains("phiRange")) {
      phiRange = true;
      Array* a = hash->getValue("phiRange")->getArray();
      minPhi = a->at(0)->getDouble() * DEG_TO_RAD;
      maxPhi = a->at(1)->getDouble() * DEG_TO_RAD;
   }
   
   bbox.x0 = bbox.z0 = -a - b;
   bbox.y0 = -b;
   bbox.x1 = bbox.z1 = a + b;
   bbox.y1 = b;
}

bool Torus::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   if(!bbox.hit(ray)) {
      return false;
   }

   double ox = ray.origin.x;     double oy = ray.origin.y;     double oz = ray.origin.z;
   double dx = ray.direction.x;  double dy = ray.direction.y;  double dz = ray.direction.z;

   double sum_d_sqrd = dx * dx + dy * dy + dz * dz;
   double e = ox * ox + oy * oy + oz * oz - a * a + b * b;
   double f = ox * dx + oy * dy + oz * dz;
   double fourSizeSquared = 4.0 * a * a;
   
   double coeffs[5];
   double roots[4];
   
   // Define the coefficients
   coeffs[0] = e * e - fourSizeSquared * (b * b - oy * oy);
   coeffs[1] = 4.0 * f * e + 2.0 * fourSizeSquared * oy * dy;
   coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + fourSizeSquared * dy * dy;
   coeffs[3] = 4.0 * sum_d_sqrd * f;
   coeffs[4] = sum_d_sqrd * sum_d_sqrd;
   
   // Find the roots
   int numRoots = SolveQuartic(coeffs, roots);
   
   if(numRoots == 0) {
      // Ray misses torus
      return false;
   }
   
   bool intersected = false;
   double t = 10.0e10;
   
   // Find smallest root > epsilon if any
   for(int j = 0; j < numRoots; j++) {
      if(roots[j] > epsilon && partCheck(ray, roots[j])) {
         intersected = true;
         if(roots[j] < t) {
            t = roots[j];
         }
      }
   }
   
   if(!intersected) {
      return false;
   }
   
   tmin = t;
   sr.localHitPoint = ray.origin + ray.direction * t;
   computeNormal(sr.normal, sr.localHitPoint);
   
   if((-ray.direction).dot(sr.normal) < 0.0) {
      sr.normal *= -1;
   }

   return true;
}

bool Torus::shadowHit(const Ray& ray, double& tmin) const {
   if(!bbox.hit(ray)) {
      return false;
   }

   double ox = ray.origin.x;     double oy = ray.origin.y;     double oz = ray.origin.z;
   double dx = ray.direction.x;  double dy = ray.direction.y;  double dz = ray.direction.z;

   double sum_d_sqrd = dx * dx + dy * dy + dz * dz;
   double e = ox * ox + oy * oy + oz * oz - a * a + b * b;
   double f = ox * dx + oy * dy + oz * dz;
   double fourSizeSquared = 4.0 * a * a;

   double coeffs[5];
   double roots[4];

   // Define the coefficients
   coeffs[0] = e * e - fourSizeSquared * (b * b - oy * oy);
   coeffs[1] = 4.0 * f * e + 2.0 * fourSizeSquared * oy * dy;
   coeffs[2] = 2.0 * sum_d_sqrd * e + 4.0 * f * f + fourSizeSquared * dy * dy;
   coeffs[3] = 4.0 * sum_d_sqrd * f;
   coeffs[4] = sum_d_sqrd * sum_d_sqrd;

   // Find the roots
   int numRoots = SolveQuartic(coeffs, roots);

   if(numRoots == 0) {
      // Ray misses torus
      return false;
   }

   bool intersected = false;
   double t = 10.0e10;

   // Find smallest root > epsilon if any
   for(int j = 0; j < numRoots; j++) {
      if(roots[j] > epsilon && partCheck(ray, roots[j])) {
         intersected = true;
         if(roots[j] < t) {
            t = roots[j];
         }
      }
   }

   if(!intersected) {
      return false;
   }

   tmin = t;
   return true;
}

void Torus::computeNormal(Vector3D& n, const Point3D& p) const {
   double v = p.x* p.x + p.y * p.y + p.z * p.z - (a * a + b * b);
   n.x = 4.0 * p.x * v;
   n.y = 4.0 * p.y * (v + 2.0 * a * a);
   n.z = 4.0 * p.z * v;
   n.normalize();
}

bool Torus::partCheck(const Ray& ray, double t) const {
   if(!thetaRange && !phiRange) {
      return true;
   }
   
   Point3D hit = ray.origin + ray.direction * t;

   if(thetaRange) {
      if(hit.y > a * cosThetaMin || hit.y < a * cosThetaMax) {
         return false;
      }
   }
   
   if(phiRange) {
      double phi = atan2(hit.x, hit.z);
      if(phi < 0.0) {
         phi += 2.0 * M_PI;
      }
      if(phi < minPhi || phi > maxPhi) {
         return false;
      }
   }

   return true;
}

void Torus::setThetaRange(double min, double max) {
   thetaRange = true;
   minTheta = min * DEG_TO_RAD;
   maxTheta = max * DEG_TO_RAD;
}

void Torus::setPhiRange(double min, double max) {
   phiRange = true;
   minPhi = min * DEG_TO_RAD;
   maxPhi = max * DEG_TO_RAD;
}
