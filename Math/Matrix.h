/*
 *  Matrix.h
 *  RayTracer
 *
 *  Created by Eric Saari on 1/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Point3D.h"
#include "Vector3D.h"

class Matrix {
   
public:
   double m[3][4];
   
   Matrix();
   void setIdentity();

   void translate(double x, double y, double z);
   void invTranslate(double x, double y, double z);

   void scale(double x, double y, double z);
   void invScale(double x, double y, double z);
   
   void rotateX(double angle);
   void invRotateX(double angle);
   
   void rotateY(double angle);
   void invRotateY(double angle);
   
   void rotateZ(double angle);
   void invRotateZ(double angle);
   
   Point3D operator*(const Point3D& p) const;
   Vector3D operator*(const Vector3D& p) const;
   Vector3D transformNormal(const Vector3D& p) const;
};




#endif
