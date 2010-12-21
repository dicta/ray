/*
 *  Regular.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Regular.h"
#include <math.h>

Regular::Regular(const int num) : Sampler(num) {
   generateSamples();
}

Regular::~Regular() {
}

void Regular::generateSamples() {
   int n = (int) sqrt((float) numSamples);
   
   for(int j = 0; j < numSets; j++) {
      for(int p = 0; p < n; p++) {
         for(int q = 0; q < n; q++) {
            samples.push_back(new Point2D((q + 0.5) / n, (p + 0.5) / n));
         }
      }
   }
}
