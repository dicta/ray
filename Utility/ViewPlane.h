/*
 *  ViewPlane.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VIEWPLANE_H_
#define _VIEWPLANE_H_

class ViewPlane {
   
public:
   ViewPlane(int w = 100, int h = 100, int _scale = 1);

   int width;
   int height;
   int scale;
};

#endif
