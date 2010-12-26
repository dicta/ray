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

void Pinhole::renderScene() {
   Color pixelColor;
   Ray ray;
   double x, y;
   
   ray.origin = eye;
   
   SDL_LockSurface(surface);

   for(int r = 0; r < height; r++) {
      for(int c = 0; c < width; c++) {
         pixelColor = BLACK;

         for(int j = 0; j < sampler->getNumSamples(); j++) {
            Point2D* sp = sampler->sampleUnitSquare();
            x = c - 0.5 * width + sp->x;
            y = r - 0.5 * height + sp->y;
            ray.direction = u * x + v * y - w * viewPlaneDistance;
            ray.direction.normalize();
            
            pixelColor += tracer->traceRay(ray);
         }
         
         pixelColor /= sampler->getNumSamples();
         pixelColor.normalize();
         setPixel(c, height - r - 1, pixelColor);
      }
   }

   SDL_UnlockSurface(surface);
   SDL_UpdateRect(surface, 0, 0, width, height);
}
