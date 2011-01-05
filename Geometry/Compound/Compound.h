#ifndef _COMPOUND_H_
#define _COMPOUND_H_

#include "Geometry/GeometryObject.h"
#include "Geometry/BBox.h"
#include <vector>

using namespace std;

typedef vector<GeometryObject*>::const_iterator GeometryIter;

class Compound : public GeometryObject {
   
public:
   Compound() {}
   virtual ~Compound();
   
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
protected:
   vector<GeometryObject*> objects;
   BBox bbox;
   
   GeometryIter begin() const { return objects.begin(); }
   GeometryIter end() const { return objects.end(); }
};

#endif
