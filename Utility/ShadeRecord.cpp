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
   material(),
   depth(0),
   samplePoint(),
   lightNormal(),
   wi(),
   tracer(NULL),
   tu(0),
   tv(0),
   dpdu(),
   dpdv()
{
}

ShadeRecord::~ShadeRecord() {
   tracer = NULL;
}
