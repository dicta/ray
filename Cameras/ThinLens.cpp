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
#include "Utility/SDL_Utility.h"

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

void ThinLens::renderScene(SDL_Rect& rect) {
   Color pixelColor;
   Ray ray;
   Point2D lp;

   SDL_Surface* s = createSurface(rect);
   SDL_LockSurface(s);

   for(int r = rect.y, o = rect.y + rect.h - 1; r < rect.y + rect.h; r++, o--) {
      for(int c = rect.x; c < rect.x + rect.w; c++) {
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

            pixelColor += tracer->traceRay(ray, 1);
         }

         pixelColor /= sampler->getNumSamples();
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
