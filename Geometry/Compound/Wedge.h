#ifndef _WEDGE_H_
#define _WEDGE_H_

#include "Compound.h"

class Hash;

class Wedge : public Compound {

public:
   Wedge();
   virtual ~Wedge();
   
   virtual void setHash(Hash* hash);
};

#endif
