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

class Tracer;
class Sampler;
class Hash;

class Camera {

public:
   Camera();
   virtual ~Camera();
   
   virtual void setHash(Hash* hash);

   void computeUVW();
   virtual void renderScene() = 0;
   
   void setEye(Array* a) { eye.set(a); }
   void setLookat(Array* a) { lookat.set(a); }
   void setUp(Array* a) { up.set(a); }
   
   void setSurface(SDL_Surface* s) { surface = s; }
   void setWidth(const int w) { width = w; }
   void setHeight(const int h) { height = h; }
   void setPixelSize(const float s) { pixelSize = s; }
   
protected:
   void setPixel(int x, int y, const Color& color);

   Point3D eye;
   Point3D lookat;
   Vector3D up;
   Vector3D u, v, w;
   Tracer* tracer;
   Sampler* sampler;
   SDL_Surface* surface;
   
   int width;
   int height;
   float pixelSize;
};

#endif
