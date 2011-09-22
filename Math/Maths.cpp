/*
 *  Maths.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/28/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Maths.h"

float normalize(const float f) {
   float g = f;
   while(g < 0.0) {
      g += 1.0;
   }
   while(g > 1.0) {
      g -= 1.0;
   }
   return g;
}

double smoothStep(double a, double b, double x) {
   if(x < a) {
      return 0.0;
   }
   if(x >= b) {
      return 1.0;
   }

   // y is the amount x is between a and b as a percentage. [0, 1]
   double y = (x - a) / (b - a);
   return y * y * (3.0 - 2.0 * y);
}

/** Modulus for doubles. */
double mod(double a, double b) {
   int n = (int)(a / b);
   a -= n * b;
   if(a < 0.0) {
      a += b;
   }
   return a;
}

void coordinateSystem(const Vector3D& v1, Vector3D* v2, Vector3D* v3) {
   if(fabs(v1.x) > fabs(v1.y)) {
      double invLen = 1.0 / sqrt(v1.x * v1.x + v1.z * v1.z);
      *v2 = Vector3D(-v1.z * invLen, 0.0, v1.x * invLen);
   }
   else {
      double invLen = 1.0 / sqrt(v1.y * v1.y + v1.z * v1.z);
      *v2 = Vector3D(0.0, v1.z * invLen, -v1.y * invLen);
   }
   *v3 = v1.cross(*v2);
}
