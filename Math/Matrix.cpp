/*
 *  Matrix.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 1/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Matrix.h"
#include "Math/Maths.h"

Matrix::Matrix() {
   setIdentity();
}

void Matrix::setIdentity() {
   for(int y = 0; y < 4; y++) {
      for(int x = 0; x < 3; x++) {
         m[x][y] = (x == y) ? 1.0 : 0.0;
      }
   }
}

void Matrix::translate(double x, double y, double z) {
   m[0][3] = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
   m[1][3] = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
   m[2][3] = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
}

void Matrix::invTranslate(double x, double y, double z) {
   m[0][3] = -m[0][0] * x - m[0][1] * y - m[0][2] * z + m[0][3];
   m[1][3] = -m[1][0] * x - m[1][1] * y - m[1][2] * z + m[1][3];
   m[2][3] = -m[2][0] * x - m[2][1] * y - m[2][2] * z + m[2][3];
}

void Matrix::scale(double x, double y, double z) {
   m[0][0] *= x;  m[0][1] *= y;  m[0][2] *= z;
   m[1][0] *= x;  m[1][1] *= y;  m[1][2] *= z;
   m[2][0] *= x;  m[2][1] *= y;  m[2][2] *= z;
}

void Matrix::invScale(double x, double y, double z) {
   double invX = 1.0 / x;
   double invY = 1.0 / y;
   double invZ = 1.0 / z;
   m[0][0] *= invX;  m[0][1] *= invY;  m[0][2] *= invZ;
   m[1][0] *= invX;  m[1][1] *= invY;  m[1][2] *= invZ;
   m[2][0] *= invX;  m[2][1] *= invY;  m[2][2] *= invZ;
}

void Matrix::rotateX(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);
   
   double m01 = m[0][1] * cosA + m[0][2] * sinA;
   double m02 = -m[0][1] * sinA + m[0][2] * cosA;
   double m11 = m[1][1] * cosA + m[1][2] * sinA;
   double m12 = -m[1][1] * sinA + m[1][2] * cosA;
   double m21 = m[2][1] * cosA + m[2][2] * sinA;
   double m22 = -m[2][1] * sinA + m[2][2] * cosA;
   double m31 = m[3][1] * cosA + m[3][2] * sinA;
   double m32 = -m[3][1] * sinA + m[3][2] * cosA;

   m[0][1] = m01;
   m[0][2] = m02;
   m[1][1] = m11;
   m[1][2] = m12;
   m[2][1] = m21;
   m[2][2] = m22;
   m[3][1] = m31;
   m[3][2] = m32;
}

void Matrix::invRotateX(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);
   
   double m01 = m[0][1] * cosA - m[0][2] * sinA;
   double m02 = m[0][1] * sinA + m[0][2] * cosA;
   double m11 = m[1][1] * cosA - m[1][2] * sinA;
   double m12 = m[1][1] * sinA + m[1][2] * cosA;
   double m21 = m[2][1] * cosA - m[2][2] * sinA;
   double m22 = m[2][1] * sinA + m[2][2] * cosA;
   double m31 = m[3][1] * cosA - m[3][2] * sinA;
   double m32 = m[3][1] * sinA + m[3][2] * cosA;

   m[0][1] = m01;
   m[0][2] = m02;
   m[1][1] = m11;
   m[1][2] = m12;
   m[2][1] = m21;
   m[2][2] = m22;
   m[3][1] = m31;
   m[3][2] = m32;
}

void Matrix::rotateY(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);
   
   double m00 = m[0][0] * cosA - m[0][2] * sinA;
   double m02 = m[0][0] * sinA + m[0][2] * cosA;
   double m10 = m[1][0] * cosA - m[1][2] * sinA;
   double m12 = m[1][0] * sinA + m[1][2] * cosA;
   double m20 = m[2][0] * cosA - m[2][2] * sinA;
   double m22 = m[2][0] * sinA + m[2][2] * cosA;
   double m30 = m[3][0] * cosA - m[3][2] * sinA;
   double m32 = m[3][0] * sinA + m[3][2] * cosA;

   m[0][0] = m00;
   m[0][2] = m02;
   m[1][0] = m10;
   m[1][2] = m12;
   m[2][0] = m20;
   m[2][2] = m22;
   m[3][0] = m30;
   m[3][2] = m32;
}

void Matrix::invRotateY(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);
   
   double m00 = m[0][0] * cosA + m[0][2] * sinA;
   double m02 = -m[0][0] * sinA + m[0][2] * cosA;
   double m10 = m[1][0] * cosA + m[1][2] * sinA;
   double m12 = -m[1][0] * sinA + m[1][2] * cosA;
   double m20 = m[2][0] * cosA + m[2][2] * sinA;
   double m22 = -m[2][0] * sinA + m[2][2] * cosA;
   double m30 = m[3][0] * cosA + m[3][2] * sinA;
   double m32 = -m[3][0] * sinA + m[3][2] * cosA;

   m[0][0] = m00;
   m[0][2] = m02;
   m[1][0] = m10;
   m[1][2] = m12;
   m[2][0] = m20;
   m[2][2] = m22;
   m[3][0] = m30;
   m[3][2] = m32;
}

void Matrix::rotateZ(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);

   double m00 = m[0][0] * cosA + m[0][1] * sinA;
   double m01 = -m[0][0] * sinA + m[0][1] * cosA;
   double m10 = m[1][0] * cosA + m[1][1] * sinA;
   double m11 = -m[1][0] * sinA + m[1][1] * cosA;
   double m20 = m[2][0] * cosA + m[2][1] * sinA;
   double m21 = -m[2][0] * sinA + m[2][1] * cosA;
   double m30 = m[3][0] * cosA + m[3][1] * sinA;
   double m31 = -m[3][0] * sinA + m[3][1] * cosA;

   m[0][0] = m00;
   m[0][1] = m01;
   m[1][0] = m10;
   m[1][1] = m11;
   m[2][0] = m20;
   m[2][1] = m21;
   m[3][0] = m30;
   m[3][1] = m31;
}

void Matrix::invRotateZ(double angle) {
   double cosA = cos(angle * DEG_TO_RAD);
   double sinA = sin(angle * DEG_TO_RAD);

   double m00 = m[0][0] * cosA - m[0][1] * sinA;
   double m01 = m[0][0] * sinA + m[0][1] * cosA;
   double m10 = m[1][0] * cosA - m[1][1] * sinA;
   double m11 = m[1][0] * sinA + m[1][1] * cosA;
   double m20 = m[2][0] * cosA - m[2][1] * sinA;
   double m21 = m[2][0] * sinA + m[2][1] * cosA;
   double m30 = m[3][0] * cosA - m[3][1] * sinA;
   double m31 = m[3][0] * sinA + m[3][1] * cosA;
   
   m[0][0] = m00;
   m[0][1] = m01;
   m[1][0] = m10;
   m[1][1] = m11;
   m[2][0] = m20;
   m[2][1] = m21;
   m[3][0] = m30;
   m[3][1] = m31;
}

Point3D Matrix::operator*(const Point3D& p) const {
   return Point3D(m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3],
                  m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3],
                  m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3]);
}

Vector3D Matrix::operator*(const Vector3D& p) const {
   return Vector3D(m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z,
                   m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z,
                   m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z);
}

Vector3D Matrix::transformNormal(const Vector3D& p) const {
   return Vector3D(m[0][0] * p.x + m[1][0] * p.y + m[2][0] * p.z,
                   m[0][1] * p.x + m[1][1] * p.y + m[2][1] * p.z,
                   m[0][2] * p.x + m[1][2] * p.y + m[2][2] * p.z);
}

void Matrix::invert() {
   double A = m[1][1] * m[2][2] - m[1][2] * m[2][1];
   double B = m[1][2] * m[2][0] - m[2][2] * m[1][0];
   double C = m[1][0] * m[2][1] - m[1][1] * m[2][0];
   
   double det = m[0][0] * A + m[0][1] * B + m[0][2] * C;
}
