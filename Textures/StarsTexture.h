/*
 *  StarsTexture.h
 *  RayTracer
 *
 *  Created by Eric Saari on 5/14/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _STARS_TEXTURE_H_
#define _STARS_TEXTURE_H_

#include "Texture.h"
#include <math.h>

struct Star {
   bool contains(const Point3D& p) const { return fabs((center - p).length()) <= radius; }

   Point3D center;
   float radius;
   Color color;
};

class Quadrant {
   
public:
   void addStar(Star* s) { stars.push_back(s); }
   Star* hitStar(const Point3D& p) const;
   
private:
   vector<Star*> stars;
};

class StarsTexture : public Texture {
   
public:
   StarsTexture();
   virtual ~StarsTexture();
   
   virtual Color getColor(const ShadeRecord& sr) const;
   virtual void setHash(Hash* hash);
   
private:
   void createStars();
   void createStars(int a, int b, int c);

   Quadrant* quads;
};

#endif
