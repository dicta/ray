#ifndef _STARBOX_H_
#define _STARBOX_H_

#include "Materials/Emissive.h"

class Hash;

class StarBox {
   
public:
   StarBox();
   
   void setHash(Hash* h);
   void createStars();
   
private:
   int count;
   int halfSize;
   int size;
   
   void createBoxSide(string name, const Point3D& origin, const Vector3D& a, const Vector3D& b);
};

#endif
