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
#include "Math/Maths.h"
#include <math.h>

typedef struct {
   Camera* camera;
   SDL_Rect rect;
} ThreadStruct;

void* thread(void* arg) {
   Uint32 start = SDL_GetTicks();
   
   ThreadStruct* ts = (ThreadStruct*) arg;
   ts->camera->renderScene(ts->rect);
   delete ts;
   
   Uint32 stop = SDL_GetTicks();
   printf("Thread time = %f seconds\n", (stop - start) / 1000.0);
   
   return 0;
}

Camera::Camera(int w, int h) : eye(), lookat(), up(), width(w), height(h) {
   pthread_mutex_init(&surfLock, NULL);
}

Camera::~Camera() {
   delete sampler;
   delete tracer;
   pthread_mutex_destroy(&surfLock);
}

void Camera::setHash(Hash* h) {   
   eye.set(h->getValue("eye")->getArray());
   lookat.set(h->getValue("lookat")->getArray());
   up.set(h->getValue("up")->getArray());

   float angle = h->getDouble("angle");
   viewPlaneDistance = height * 0.5 / tan(angle * DEG_TO_RAD);
   
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
   
   if(h->contains("bgTexture")) {
      Texture* tex = Texture::createTexture(h->getValue("bgTexture")->getHash());
      tracer->setBackgroundTexture(tex);
   }
}

void Camera::render() {
   for(int h = 0; h < height; h += 100) {
      for(int w = 0; w < width; w += 100) {
         SDL_Rect rect;
         rect.w = 100;
         rect.h = 100;
         rect.x = w;
         rect.y = h;
         
         ThreadStruct* ts = new ThreadStruct;
         ts->camera = this;
         ts->rect = rect;
         
         pthread_t tid;
         pthread_create(&tid, NULL, thread, (void *) ts);
      }
   }
}

void Camera::computeUVW() {
   if(eye.x == lookat.x && eye.z == lookat.z && eye.y > lookat.y) {
      u.set(1, 0, 0);
      v.set(0, 0, 1);
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

void Camera::setPixel(SDL_Surface* s, int x, int y, const Color& color) {   
   int bpp = s->format->BytesPerPixel;
   /* Here p is the address to the pixel we want to set */
   Uint8 *p = (Uint8 *)s->pixels + y * s->pitch + x * bpp;
   Uint32 pixel = SDL_MapRGB(s->format, color.getRed(), color.getGreen(), color.getBlue());
   *(Uint32 *)p = pixel;
}

SDL_Surface* Camera::createSurface(const SDL_Rect& rect) {
   SDL_Surface *surface;
   Uint32 rmask, gmask, bmask, amask;
   
   /* SDL interprets each pixel as a 32-bit number, so our masks must depend
    on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
   rmask = 0xff000000;
   gmask = 0x00ff0000;
   bmask = 0x0000ff00;
   amask = 0x000000ff;
#else
   rmask = 0x000000ff;
   gmask = 0x0000ff00;
   bmask = 0x00ff0000;
   amask = 0xff000000;
#endif
   
   surface = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
   if(surface == NULL) {
      fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
      exit(1);
   }
   
   return surface;
}
