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
   Pinhole(int w, int h);
   virtual ~Pinhole() {}

   virtual void renderScene(SDL_Rect& rect);
};

#endif
