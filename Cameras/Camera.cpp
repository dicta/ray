/*
 *  Camera.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Camera.h"
#include "Samplers/MultiJittered.h"
#include "Samplers/Regular.h"
#include "Tracer/MultiObjects.h"
#include "Tracer/RayCast.h"
#include "Tracer/AreaLighting.h"
#include "Parser/Hash.h"

Camera::Camera() : eye(), lookat(), up() {
   pixelSize = 1.0;
}

Camera::~Camera() {
   delete sampler;
   delete tracer;
}

void Camera::setHash(Hash* h) {   
   eye.set(h->getValue("eye")->getArray());
   lookat.set(h->getValue("lookat")->getArray());
   up.set(h->getValue("up")->getArray());
   pixelSize = h->getDouble("pixelSize");
   
   int numSamples = h->getInteger("numSamples");
   if(numSamples == 1) {
      sampler = new Regular();
   }
   else {
      sampler = new MultiJittered(numSamples);
   }
   
   string t = h->getString("tracer");
   if(t == "area") {
      tracer = new AreaLighting();
   }
   else if(t == "rayCast") {
      tracer = new RayCast();
   }
}

void Camera::computeUVW() {
   if(eye.x == lookat.x && eye.z == lookat.z && eye.y > lookat.y) {
      u.set(0, 0, 1);
      v.set(1, 0, 0);
      w.set(0, 1, 0);
   }
   else {
      up.normalize();

      w = eye - lookat;
      w.normalize();
      u = up.cross(w);
      u.normalize();
      v = w.cross(u);
   }
}

void Camera::setPixel(int x, int y, const Color& color) {   
   int bpp = surface->format->BytesPerPixel;
   /* Here p is the address to the pixel we want to set */
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
   Uint32 pixel = SDL_MapRGB(surface->format, color.getRed(), color.getGreen(), color.getBlue());
   *(Uint32 *)p = pixel;
}
