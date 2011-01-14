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
#include <queue>

using namespace std;

queue<SDL_Rect> rects;
pthread_mutex_t rectLock;
int threadCount;

void* renderThread(void* arg) {
   Camera* c = (Camera*) arg;
   SDL_Rect r;
   
   while(true) {
      pthread_mutex_lock(&rectLock);
      if(rects.empty()) {
         pthread_mutex_unlock(&rectLock);
         pthread_exit(NULL);
      }
      r = rects.front();
      rects.pop();
      pthread_mutex_unlock(&rectLock);
      
      c->renderScene(r);
   }
   pthread_exit(NULL);
}

void* timerThread(void* arg) {
   pthread_attr_t attr;
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   
   Uint32 start = SDL_GetTicks();
   
   pthread_t tid;
   for(int i = 0; i < threadCount; i++) {
      pthread_create(&tid, &attr, renderThread, arg);
      pthread_join(tid, NULL);
   }
   
   pthread_attr_destroy(&attr);
   
   Uint32 end = SDL_GetTicks();
   printf("Runtime = %f seconds\n", (end - start) / 1000.0);
   
   pthread_exit(NULL);
}

Camera::Camera(int w, int h) : eye(), lookat(), up(), width(w), height(h), boxw(0), boxh(0) {
   pthread_mutex_init(&surfLock, NULL);
   pthread_mutex_init(&rectLock, NULL);
   threadCount = 1;
}

Camera::~Camera() {
   delete sampler;
   delete tracer;
   pthread_mutex_destroy(&surfLock);
   pthread_mutex_destroy(&rectLock);
}

void Camera::setThreadParameters(int tc, int w, int h) {
   threadCount = tc;
   boxw = w;
   boxh = h;
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
   else if(h->contains("bgColor")) {
      tracer->setBackgroundColor(h->getValue("bgColor")->getArray());
   }
}

void Camera::render() {
   for(int h = 0; h < height; h += boxh) {
      for(int w = 0; w < width; w += boxw) {
         SDL_Rect rect;
         rect.w = boxw;
         rect.h = boxh;
         rect.x = w;
         rect.y = h;
         rects.push(rect);
      }
   }

   pthread_t tid;
   pthread_create(&tid, NULL, timerThread, (void *) this);
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
