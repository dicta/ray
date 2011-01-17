/*
 *  Pinhole.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>
#include <SDL/SDL.h>
#include "Pinhole.h"
#include "Utility/Color.h"
#include "Math/Ray.h"
#include "Tracer/Tracer.h"
#include "Samplers/Sampler.h"
#include "Math/Point2D.h"
#include "Parser/Hash.h"

Pinhole::Pinhole(int w, int h) : Camera(w, h) {
}

void Pinhole::renderScene(SDL_Rect& rect) {
   Color pixelColor;
   Ray ray;
   double x, y;
   
   ray.origin = eye;
   
   SDL_Surface* s = createSurface(rect);
   SDL_LockSurface(s);

   for(int r = rect.y, o = rect.y + rect.h - 1; r < rect.y + rect.h; r++, o--) {
      for(int c = rect.x; c < rect.x + rect.w; c++) {
         pixelColor = BLACK;

         for(int j = 0; j < sampler->getNumSamples(); j++) {
            Point2D* sp = sampler->sampleUnitSquare();
            x = c - 0.5 * width + sp->x;
            y = r - 0.5 * height + sp->y;
            ray.direction = u * x + v * y - w * viewPlaneDistance;
            ray.direction.normalize();
            
            pixelColor += tracer->traceRay(ray, 0);
         }
         
         pixelColor /= sampler->getNumSamples();
         pixelColor.normalize();
         setPixel(s, c - rect.x, o - rect.y, pixelColor);
      }
   }

   SDL_UnlockSurface(s);
   rect.y = height - rect.h - rect.y;
   
   pthread_mutex_lock(&surfLock);
   SDL_BlitSurface(s, NULL, surface, &rect);
   SDL_UpdateRect(surface, 0, 0, width, height);
   pthread_mutex_unlock(&surfLock);
   
   SDL_FreeSurface(s);
}
