#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "Geometry/GeometryObject.h"
#include "Geometry/Compound/Compound.h"

using namespace std;

class Storage : public Compound {
   
public:
   Storage();
   virtual ~Storage() {}
   virtual void setup() {}
   virtual void addObject(GeometryObject* obj);

   void removeObject(GeometryObject* obj);
};

#endif
