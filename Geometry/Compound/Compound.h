#ifndef _COMPOUND_H_
#define _COMPOUND_H_

#include "Geometry/GeometryObject.h"
#include <list>

using namespace std;

typedef list<GeometryObject*>::const_iterator CompoundIter;

class Compound : public GeometryObject {
   
public:
   Compound();
   virtual ~Compound();
   
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   virtual void addObject(GeometryObject* obj);

   virtual bool isCompound() { return true; }
   virtual list<GeometryObject*> getObjects() const { return objs; }
   
protected:
   list<GeometryObject*> objs;
};

#endif
