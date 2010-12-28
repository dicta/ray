/*
 *  MultiJittered.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _MULTI_JITTERED_H_
#define _MULTI_JITTERED_H_

#include "Sampler.h"

class MultiJittered : public Sampler {
   
public:
   MultiJittered(const int ns = 1, const int sets = 83);
   ~MultiJittered();
   
   virtual void generateSamples();
};

#endif
