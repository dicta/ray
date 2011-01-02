/*
 *  ThinLens.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _THIN_LENS_H_
#define _THIN_LENS_H_

#include "Camera.h"

class Sampler;
class Hash;

class ThinLens : public Camera {
   
public:
   ThinLens(int w, int h);
   virtual ~ThinLens();
   
   virtual void setHash(Hash* hash);
   virtual void renderScene(SDL_Rect& rect);
   
private:
   float lensRadius;
   // Distance from eye to focal plane
   float f;
   
   Sampler* lensSampler;
};

#endif
