#ifndef _WEDGE_H_
#define _WEDGE_H_

#include "Compound.h"

class Hash;

class Wedge : public Compound {

public:
   Wedge();
   Wedge(float ir, float outr, float br, float a1, float a2, float y1, float y2);
   virtual ~Wedge();
   
   virtual void setHash(Hash* hash);
   
private:
   void build();

   float innerR;
   float outerR;
   float bevelR;
   float angle1;
   float angle2;
   float minY;
   float maxY;
   bool cover;
};

#endif
