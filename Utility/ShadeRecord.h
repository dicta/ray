/*
 *  ShadeRecord.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SHADE_RECORD_H_
#define _SHADE_RECORD_H_

#include "Math/Vector3D.h"
#include "Math/Point3D.h"

class Material;
class Tracer;

class ShadeRecord {
   
public:
   ShadeRecord();
   ~ShadeRecord();

   Vector3D normal;
   Point3D hitPoint;
   Point3D localHitPoint;
   Material* material;
   bool hit;
   double t;
   int depth;
   
   // The following are used by area lights
   Point3D* samplePoint;
   Vector3D* lightNormal;
   Vector3D* wi;
   Tracer* tracer;
   
   double tu, tv;
};

#endif
