#ifndef _REGULAR_H_
#define _REGULAR_H_

#include "Sampler.h"

class Regular : public Sampler {

public:
   Regular(const int num = 1);
   virtual ~Regular();
   
   virtual void generateSamples();
};

#endif
