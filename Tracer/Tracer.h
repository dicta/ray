/*
 *  Tracer.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TRACER_H_
#define _TRACER_H_

#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Ray.h"
#include "Textures/Texture.h"

class Tracer {

public:
   Tracer() : bgColor(), texture(NULL), maxDepth(10) {}
   virtual ~Tracer() {
      if(texture != NULL) {
         delete texture;
      }
   }

   virtual Color traceRay(const Ray& ray, const int depth) = 0;

   void setBackgroundColor(const Color& c) { bgColor = c; }
   void setBackgroundTexture(Texture* t) { texture = t; }
   void setMaxDepth(int d) { maxDepth = d; }

protected:
   Color bgColor;
   Texture* texture;
   int maxDepth;
};

#endif
