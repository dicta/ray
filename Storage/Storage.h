#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "Geometry/GeometryObject.h"
#include <list>

using namespace std;

class Storage : public GeometryObject {
   
public:
   Storage();
   virtual ~Storage() {}
   virtual void setup() = 0;

   void addObject(GeometryObject* obj);
   void removeObject(GeometryObject* obj);
   
protected:
   list<GeometryObject*> objs;
};

#endif
