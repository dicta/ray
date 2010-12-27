#include "Torus.h"
#include "Math/Maths.h"
#include "Parser/Hash.h"
#include <math.h>

Torus::Torus() : GeometryObject(), bbox() {
}

void Torus::setHash(Hash* hash) {
   a = hash->getDouble("a");
   b = hash->getDouble("b");
   setupMaterial(hash->getValue("material")->getHash());
   
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
      if(roots[j] > epsilon) {
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
      if(roots[j] > epsilon) {
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
