#include "Triangle.h"

Triangle::Triangle(const Point3D& p1, const Point3D& p2, const Point3D& p3) :
   v0(p1), v1(p2), v2(p3)
{
   normal = (v1 - v0).cross(v2 - v0);
   normal.normalize();
}

Triangle::~Triangle() {
}

bool Triangle::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.direction.x, d = v0.x - ray.origin.x;
   double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.direction.y, h = v0.y - ray.origin.y;
   double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.direction.z, l = v0.z - ray.origin.z;
   
   double m = f * k -g * j, n = h * k -g * l, p = f * l - h * j;
   double q = g * i -e * k, s = e * j - f * i;
   
   double invDenom = 1.0 / (a * m + b * q + c * s);
   
   double e1 = d * m - b * n - c * p;
   double beta = e1 * invDenom;
   
   if(beta < 0.0) {
      return false;
   }
   
   double r = e * l - h * i;
   double e2 = a * n + d * q + c * r;
   double gamma = e2 * invDenom;
   
   if(gamma < 0.0) {
      return false;
   }
   
   if(beta + gamma > 1.0) {
      return false;
   }
   
   double e3 = a * p - b * r + d * s;
   double t = e3 * invDenom;
   
   if(t < epsilon) {
      return false;
   }
   
   tmin = t;
   sr.normal = normal;
   sr.localHitPoint = ray.origin + ray.direction * t;
   return true;
}

bool Triangle::shadowHit(const Ray& ray, double& tmin) const {
   double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.direction.x, d = v0.x - ray.origin.x;
   double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.direction.y, h = v0.y - ray.origin.y;
   double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.direction.z, l = v0.z - ray.origin.z;
   
   double m = f * k -g * j, n = h * k -g * l, p = f * l - h * j;
   double q = g * i -e * k, s = e * j - f * i;
   
   double invDenom = 1.0 / (a * m + b * q + c * s);
   
   double e1 = d * m - b * n - c * p;
   double beta = e1 * invDenom;
   
   if(beta < 0.0) {
      return false;
   }
   
   double r = e * l - h * i;
   double e2 = a * n + d * q + c * r;
   double gamma = e2 * invDenom;
   
   if(gamma < 0.0) {
      return false;
   }
   
   if(beta + gamma > 1.0) {
      return false;
   }
   
   double e3 = a * p - b * r + d * s;
   double t = e3 * invDenom;
   
   if(t < epsilon) {
      return false;
   }
   
   tmin = t;
   return true;
}
