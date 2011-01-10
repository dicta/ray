#ifndef _WEDGE_RING_H_
#define _WEDGE_RING_H_

#include "Wedge.h"

class WedgeRing : public Compound {

public:
   WedgeRing();
   virtual ~WedgeRing();
   
   virtual void setHash(Hash* hash);
};

#endif
