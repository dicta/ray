#ifndef _COMPOUND_H_
#define _COMPOUND_H_

#include "Geometry/GeometryObject.h"
#include <vector>

using namespace std;

typedef vector<GeometryObject*>::const_iterator CompoundIter;

class Compound : public GeometryObject {
   
public:
   Compound();
   virtual ~Compound();
   
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   void addObject(GeometryObject* obj);

   virtual bool isCompound() { return true; }
   virtual vector<GeometryObject*> getObjects() const { return objects; }
   
protected:
   vector<GeometryObject*> objects;
};

#endif
