#include "Vector3D.h"
#include "Point3D.h"
#include <math.h>

static double ERROR = 0.0001;

Vector3D::Vector3D(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {
}

Vector3D::Vector3D(const Vector3D &v) {
   set(v.x, v.y, v.z);
}

void Vector3D::set(double xx, double yy, double zz) {
   x = xx;
   y = yy;
   z = zz;
}

void Vector3D::set(Array* a) {
   x = a->at(0)->getDouble();
   y = a->at(1)->getDouble();
   z = a->at(2)->getDouble();
}

Point3D Vector3D::operator+ (const Point3D& p) {
   Point3D result;
   result.set(x + p.x, y + p.y, z + p.z);
   return result;
}

Vector3D Vector3D::operator+ (const Vector3D& p) const {
   Vector3D result;
   result.set(x + p.x, y + p.y, z + p.z);
   return result;
}

Vector3D& Vector3D::operator+= (const Vector3D& p) {
   this->set(x + p.x, y + p.y, z + p.z);
   return *this;
}

Vector3D Vector3D::operator- (const Vector3D& p) const {
   Vector3D result;
   result.set(x - p.x, y - p.y, z - p.z);
   return result;
}

Vector3D Vector3D::operator* (const double d) const {
   Vector3D result;
   result.set(x * d, y * d, z * d);
   return result;
}

Vector3D Vector3D::operator/ (const double d) const {
   Vector3D result;
   result.set(x / d, y / d, z / d);
   return result;
}

Vector3D& Vector3D::operator*= (const double d) {
   this->set(x * d, y * d, z * d);
   return *this;
}

Vector3D& Vector3D::operator= (const Vector3D& p) {
   this->set(p.x, p.y, p.z);
   return *this;
}

bool Vector3D::operator== (const Vector3D& v) {
   if(fabs(x - v.x) > ERROR) {
      return false;
   }
   if(fabs(y - v.y) > ERROR) {
      return false;
   }
   if(fabs(z - v.z) > ERROR) {
      return false;
   }
   return true;
}

double Vector3D::length() {
   return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector3D& Vector3D::normalize() {
   *this = *this * (1.0 / this->length());
   return *this;
}

double Vector3D::dot(const Vector3D& v) const {
   return (x * v.x) + (y * v.y) + (z * v.z);
}

Vector3D Vector3D::cross(const Vector3D& v) const {
  return Vector3D(
    (y * v.z) - (z * v.y),  //x component
    -((x * v.z) - (z * v.x)), //y component
    (x * v.y) - (y * v.x)   //z component
  );
}

Vector3D Vector3D::cross(const double _x, const double _y, const double _z) const {
   return Vector3D(
                   (y * _z) - (z * _y),  //x component
                   -((x * _z) - (z * _x)), //y component
                   (x * _y) - (y * _x)   //z component
                   );                   
}

Vector3D& Vector3D::selfCross(const Vector3D& v) {
  double nx = (y * v.z) - (z * v.y);
  double ny = (z * v.x) - (x * v.z);
  double nz = (x * v.y) - (y * v.x);

  this->set(nx, ny, nz);
  return *this;
}
