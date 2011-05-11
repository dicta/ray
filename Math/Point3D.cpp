#include "Point3D.h"
#include <stdio.h>
#include <math.h>

static double ERROR = 0.0001;

Point3D::Point3D(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {
}

Point3D::Point3D(const Point3D& p) {
   set(p.x, p.y, p.z);
}

Point3D::Point3D(Array* a) {
   set(a);
}

void Point3D::set(double xx, double yy, double zz) {
   x = xx;
   y = yy;
   z = zz;
}

void Point3D::set(Array* a) {
   x = a->at(0)->getDouble();
   y = a->at(1)->getDouble();
   z = a->at(2)->getDouble();
}

double Point3D::distanceSquared(const Point3D& p) const {
	return (	(x - p.x) * (x - p.x) 
           + 	(y - p.y) * (y - p.y)
           +	(z - p.z) * (z - p.z) );
}

Point3D& Point3D::operator+= (const Vector3D& v) {
   this->set(x + v.x, y + v.y, z + v.z);
   return *this;
}

Point3D& Point3D::operator*= (const double d) {
   set(x * d, y * d, z * d);
   return *this;
}

Point3D& Point3D::operator= (const Point3D& p) {
   this->set(p.x, p.y, p.z);
   return *this;
}

bool Point3D::operator== (const Point3D& p) const {
   if(fabs(x - p.x) > ERROR) {
      return false;
   }
   if(fabs(y - p.y) > ERROR) {
      return false;
   }
   if(fabs(z - p.z) > ERROR) {
      return false;
   }
   return true;
}

