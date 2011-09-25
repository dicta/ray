#include "Storage.h"
#include "Geometry/GeometryObject.h"
#include "Geometry/Compound/Compound.h"

Storage::Storage() : objs() {}

void Storage::addObject(GeometryObject* obj) {
  if (obj->isCompound()) {
     const vector<GeometryObject*>& objects = ((Compound*)obj)->getObjects();
     fprintf(stdout,"Adding compound object (%lu children).\n", objects.size());
     fflush(stdout);
     for(CompoundIter it = objects.begin(); it != objects.end(); it++) {
        addObject(*it);
     }
  } else {
     objs.push_back(obj);
     bbox.expand(obj->bbox);
  }
}

void Storage::removeObject(GeometryObject* obj) {
 if (obj->isCompound()) {
     printf("Removing compound object.\n");
     const vector<GeometryObject*>& objects = ((Compound*)obj)->getObjects();
     for(CompoundIter it = objects.begin(); it != objects.end(); it++) {
        removeObject(*it);
     }
  } else {
     objs.remove(obj); 
  }
}
