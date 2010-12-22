/*
 *  Texture.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Utility/ShadeRecord.h"
#include "Utility/Color.h"

class Hash;

class Texture {

public:
   virtual ~Texture() {}

   virtual Color getColor(const ShadeRecord& sr) const = 0;
   virtual void setHash(Hash* hash) = 0;
   
   static Texture* createTexture(Hash* hash);
};

#endif
