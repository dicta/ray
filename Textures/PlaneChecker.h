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

/**
 * Hash values:
 * <ul>
 * <li>color1 [ r g b ] - Primary square color
 * <li>color2 [ r g b ] - Seconday square color
 * <li>size <i>value</i> - Size of each square in world units
 * <li>borderColor [ r g b ] - Optional. Color of border around each square
 * <li>borderWidth <i>value</i> - Optional. Size of border in world units
 * </ul>
 */
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
