#ifndef _STARS_H_
#define _STARS_H_

#include "GeometryObject.h"

class Hash;

class Stars {

public:
   Stars();

   void setHash(Hash* h);
   void createStars();

private:
   void createStars(int a, int b, int c);
   
   int count;
};

#endif
