#include "Storage.h"

Storage::Storage() : objs() {}

void Storage::addObject(GeometryObject* obj) {
   objs.push_back(obj);
   bbox.expand(obj->bbox);
}

void Storage::removeObject(GeometryObject* obj) {
   objs.remove(obj); 
}
