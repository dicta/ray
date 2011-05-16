/*
 *  ShadeRecord.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ShadeRecord.h"

ShadeRecord::ShadeRecord() : 
   normal(), 
   hitPoint(), 
   localHitPoint(), 
   material(NULL), 
   hit(false), 
   t(0), 
   depth(0),
   samplePoint(NULL),
   lightNormal(NULL),
   wi(NULL),
   tracer(NULL),
   tu(0),
   tv(0)
{
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
