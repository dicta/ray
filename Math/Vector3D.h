#ifndef _VECTOR3D_H
#define _VECTOR3D_H

#include "Parser/Value.h"
#include <math.h>

class Point3D;

class Vector3D {

public:
   Vector3D(double xx = 0.0, double yy = 0.0, double zz = 0.0);
   Vector3D(const Vector3D &v);
   Vector3D(Array* a);

   void set(double xx, double yy, double zz);
   void set(Array* a);

   Vector3D operator-() const;

   Vector3D operator+(const Vector3D& p) const;
   Vector3D& operator+=(const Vector3D& p);
   Vector3D operator-(const Vector3D& p) const;
   Vector3D operator*(const double d) const;
   Vector3D operator/(const double d) const;
   Vector3D& operator*=(const double d);
   Vector3D& operator=(const Vector3D& p);
   bool operator==(const Vector3D& v);

   Vector3D& normalize();
   Vector3D absValue() const;
   double dot(const Vector3D& v) const;
   Vector3D cross(const Vector3D& v) const;
   Vector3D cross(const double _x, const double _y, const double _z) const;
   Vector3D& selfCross(const Vector3D& v);
   double length() const { return sqrt(x*x + y*y + z*z); }

   double x, y, z;
};

inline Vector3D Vector3D::operator-() const {
   return Vector3D(-x, -y, -z);
}

inline Vector3D Vector3D::operator+ (const Vector3D& p) const {
   return Vector3D(x + p.x, y + p.y, z + p.z);
}

inline Vector3D Vector3D::operator- (const Vector3D& p) const {
   return Vector3D(x - p.x, y - p.y, z - p.z);
}

inline Vector3D Vector3D::operator* (const double d) const {
   return Vector3D(x * d, y * d, z * d);
}

inline Vector3D Vector3D::operator/ (const double d) const {
   return Vector3D(x / d, y / d, z / d);
}

inline double Vector3D::dot(const Vector3D& v) const {
   return (x * v.x) + (y * v.y) + (z * v.z);
}

inline Vector3D Vector3D::cross(const Vector3D& v) const {
   return Vector3D(
                   (y * v.z) - (z * v.y),  //x component
                   (z * v.x) - (x * v.z), //y component
                   (x * v.y) - (y * v.x)   //z component
                   );
}

inline Vector3D Vector3D::cross(const double _x, const double _y, const double _z) const {
   return Vector3D(
                   (y * _z) - (z * _y),  //x component
                   (z * _x) - (x * _z), //y component
                   (x * _y) - (y * _x)   //z component
                   );
}

inline Vector3D Vector3D::absValue() const {
   return Vector3D(fabs(x), fabs(y), fabs(z));
}

#endif
