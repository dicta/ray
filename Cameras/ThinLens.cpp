/*
 *  ThinLens.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ThinLens.h"
#include "Samplers/MultiJittered.h"
#include "Math/Ray.h"
#include "Math/Point2D.h"
#include "Tracer/Tracer.h"
#include "Parser/Hash.h"

ThinLens::ThinLens(int w, int h) : Camera(w, h) {
   lensRadius = 1.0;
}

ThinLens::~ThinLens() {
   delete lensSampler;
}

void ThinLens::setHash(Hash* hash) {
   Camera::setHash(hash);
   f = hash->getDouble("focusPlaneDistance");
   lensRadius = hash->getDouble("lensRadius");

   lensSampler = new MultiJittered(hash->getInteger("lensSamples"));
   lensSampler->mapSamplesToUnitDisk();
}

void ThinLens::renderScene() {
   Color pixelColor;
   Ray ray;   
   Point2D lp;
   
   SDL_LockSurface(surface);
   
   for(int r = 0; r <= height; r++) {
      for(int c = 0; c < width; c++) {
         pixelColor = BLACK;
         
         for(int n = 0; n < sampler->getNumSamples(); n++) {
            Point2D* sp = sampler->sampleUnitSquare();
            double x = c - width / 2.0 + sp->x;
            double y = r - height / 2.0 + sp->y;
            
            Point2D* dp = lensSampler->sampleUnitDisk();
            lp = *dp * lensRadius;
            
            ray.origin = eye + u * lp.x + v * lp.y;
            
            double dirX = x * f / viewPlaneDistance;
            double dirY = y * f / viewPlaneDistance;
            ray.direction = u * (dirX - lp.x) + v * (dirY - lp.y) - w * f;
            ray.direction.normalize();
            
            pixelColor += tracer->traceRay(ray);
         }
         
         pixelColor /= sampler->getNumSamples();
         setPixel(c, height - r, pixelColor);
      }
   }
   
   SDL_UnlockSurface(surface);
   SDL_UpdateRect(surface, 0, 0, width, height);
}
