/*
 *  Camera.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <SDL/SDL.h>
#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "Utility/Color.h"
#include <pthread.h>

class Tracer;
class Sampler;
class Hash;

class Camera {

public:
   Camera(int w, int h);
   virtual ~Camera();
   
   virtual void setHash(Hash* hash);

   void render();
   virtual void renderScene(SDL_Rect& rect) = 0;
   
   void setPosition(double x, double y, double z);
   void setSurface(SDL_Surface* s) { surface = s; }
   void setThreadParameters(int tc, int w, int h);
   void rotate(double x, double y, double z);
   
protected:
   void setPixel(SDL_Surface* s, int x, int y, const Color& color);
   SDL_Surface* createSurface(const SDL_Rect& rect);
   void computeUVW(Hash* h);

   Point3D eye;
   Vector3D u, v, w;
   Tracer* tracer;
   Sampler* sampler;
   SDL_Surface* surface;
   
   int width;
   int height;
   float viewPlaneDistance;
   
   pthread_mutex_t surfLock;
   
private:
   int boxw;
   int boxh;
};

#endif
