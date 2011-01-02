/*
 *  ShadeRecord.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ShadeRecord.h"

ShadeRecord::ShadeRecord() {
   hit = false;
   samplePoint = NULL;
   lightNormal = NULL;
   wi = NULL;
}

ShadeRecord::~ShadeRecord() {
   if(samplePoint != NULL) {
      delete samplePoint;
   }
   if(lightNormal != NULL) {
      delete lightNormal;
   }
   if(wi != NULL) {
      delete wi;
   }
}
