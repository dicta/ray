/*
 *  Pinhole.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PINhOLE_H_
#define _PINhOLE_H_

#include "Camera.h"

class Pinhole : public Camera {
   
public:
   Pinhole(const float _d = 1.0);
   virtual ~Pinhole() {}
   
   virtual void setHash(Hash* h);
   
   virtual void renderScene();
   
private:
   // Distance between the eye and view plane
   float d;
};

#endif
