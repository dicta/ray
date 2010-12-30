/*
 *  Maths.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/28/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Maths.h"

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
