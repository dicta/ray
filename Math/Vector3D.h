#ifndef _VECTOR3D_H
#define _VECTOR3D_H

#ifdef WIN32
#include <GL/gl.h>
#else
#include <GLUT/glut.h>
#endif

#include "Parser/Value.h"

class Point3D;

class Vector3D {

public:
   Vector3D(double xx = 0.0, double yy = 0.0, double zz = 0.0);
   Vector3D(const Vector3D &v);

   void set(double xx, double yy, double zz);
   void set(Array* a);

   Point3D operator+(const Point3D& p);
   Vector3D operator+(const Vector3D& p) const;
   Vector3D& operator+=(const Vector3D& p);
   Vector3D operator-(const Vector3D& p);
   Vector3D operator*(const double d) const;
   Vector3D operator/(const double d) const;
   Vector3D& operator*=(const double d);
   Vector3D& operator=(const Vector3D& p);
   bool operator==(const Vector3D& v);

   Vector3D& normalize();
   double dot(const Vector3D& v) const;
   Vector3D cross(const Vector3D& v) const;
   Vector3D cross(const double _x, const double _y, const double _z) const;
   Vector3D& selfCross(const Vector3D& v);
   double length();

   GLdouble x, y, z;
};

#endif
