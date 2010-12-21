#ifndef _POINT3D_H
#define _POINT3D_H

#ifdef WIN32
#include <GL/gl.h>
#else
#include <GLUT/glut.h>
#endif

#include "Parser/Value.h"

class Vector3D;

class Point3D {

public:
   Point3D(double xx = 0.0, double yy = 0.0, double zz = 0.0);
   Point3D(Array* a);

   void set(double xx, double yy, double zz);
   void set(Array* a);
   
   double distance(const Point3D& p) const;
   double distanceSquared(const Point3D& p) const;

   Point3D operator+(const Vector3D& p) const;
   Point3D& operator+=(const Vector3D& v);
   Point3D operator-(const Vector3D& p) const;
   Vector3D operator-(const Point3D& p) const;
   Point3D operator*(const double d) const;
   Point3D& operator*=(const double d);
   Point3D& operator=(const Point3D& p);
   bool operator==(const Point3D& p) const;

   GLdouble x;
   GLdouble y;
   GLdouble z;
};

#endif
