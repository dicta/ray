/*
 *  PlaneChecker.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PLANE_CHECKER_H_
#define _PLANE_CHECKER_H_

#include "Texture.h"

class PlaneChecker : public Texture {
   
public:
   PlaneChecker();

   virtual Color getColor(const ShadeRecord& sr) const;
   virtual void setHash(Hash* hash);
   
private:
   Color color1;
   Color color2;
   float size;
   Color borderColor;
   float borderWidth;
};

#endif
