/*
 *  StarsTexture.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 5/14/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "StarsTexture.h"

typedef vector<Star*>::const_iterator StarIter;

Star* Quadrant::hitStar(const Point3D& p) const {
   for(StarIter it = stars.begin(), end = stars.end(); it != end; it++) {
      if((*it)->contains(p)) {
         return *it;
      }
   }
   return NULL;
}

StarsTexture::StarsTexture() {
   createStars();
}

StarsTexture::~StarsTexture() {
   delete quads;
}

void StarsTexture::createStars() {
   quads = new Quadrant[2 * 2 * 2];

   for(int i = 0; i < 10000; i++) {
      createStars(1, 1, 1);
      createStars(1, 1, -1);
      createStars(1, -1, 1);
      createStars(1, -1, -1);
      createStars(-1, 1, 1);
      createStars(-1, 1, -1);
      createStars(-1, -1, 1);
      createStars(-1, -1, -1);
   }
}

void StarsTexture::createStars(int a, int b, int c) {
   Star* star = new Star();

   float x = (float) rand() / (float) RAND_MAX * 10000.0;
   float y = (float) rand() / (float) RAND_MAX * 10000.0;
   float z = (float) rand() / (float) RAND_MAX * 10000.0;
   
   float r = (float) rand() / (float) RAND_MAX;
   star->color.set(max(0.0, r - 0.2), max(0.0, r - 0.2), r);
   
   star->radius = (float) rand() / (float) RAND_MAX * 0.1;
   
   Vector3D v(x * a, y * b, z * c);
   v.normalize();
   v *= 100.0;
   star->center.set(v.x, v.y, v.z);
   
   int quad = (a+1)/2 + 2 * (b+1)/2 + 2 * 2 * (c+1)/2;
   quads[quad].addStar(star);
}

Color StarsTexture::getColor(const ShadeRecord& sr) const {
   int a = sr.localHitPoint.x >= 0.0 ? 1 : -1;
   int b = sr.localHitPoint.y >= 0.0 ? 1 : -1;
   int c = sr.localHitPoint.z >= 0.0 ? 1 : -1;
   int quad = (a+1)/2 + 2 * (b+1)/2 + 2 * 2 * (c+1)/2;

   Star* star = quads[quad].hitStar(sr.localHitPoint * 100);
   if(star != NULL) {
      return star->color;
   }

   return BLACK;
}

void StarsTexture::setHash(Hash* hash) {
}
