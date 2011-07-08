#ifndef _POINT3D_H
#define _POINT3D_H

#include "Parser/Value.h"
#include "Vector3D.h"

class Point3D {

public:
   Point3D(double xx = 0.0, double yy = 0.0, double zz = 0.0);
   Point3D(const Point3D& p);
   Point3D(Array* a);

   void set(double xx, double yy, double zz);
   void set(Array* a);

   double distance(const Point3D& p) const;
   double distanceSquared(const Point3D& p) const;

   Point3D operator-() const;

   Point3D operator+(const Vector3D& p) const;
   Point3D operator+(float f) const;
   Point3D& operator+=(const Vector3D& v);

   Point3D operator-(const Vector3D& p) const;
   Vector3D operator-(const Point3D& p) const;
   Point3D operator-(float f) const;

   Point3D operator*(const double d) const;
   Point3D& operator*=(const double d);
   Point3D& operator=(const Point3D& p);
   bool operator==(const Point3D& p) const;

   double x;
   double y;
   double z;
};

inline Point3D Point3D::operator-() const {
   return Point3D(-x, -y, -z);
}

inline double Point3D::distance(const Point3D& p) const {
   return (*this - p).length();
}

inline Point3D Point3D::operator+ (const Vector3D& p) const {
   return Point3D(x + p.x, y + p.y, z + p.z);
}

inline Point3D Point3D::operator+ (float f) const {
   return Point3D(x + f, y + f, z + f);
}

inline Point3D Point3D::operator- (const Vector3D& p) const {
   return Point3D(x - p.x, y - p.y, z - p.z);
}

inline Vector3D Point3D::operator- (const Point3D& p) const {
   return Vector3D(x - p.x, y - p.y, z - p.z);
}

inline Point3D Point3D::operator- (float f) const {
   return Point3D(x - f, y - f, z - f);
}

inline Point3D Point3D::operator* (const double d) const {
   return Point3D(x * d, y * d, z * d);
}

#endif
