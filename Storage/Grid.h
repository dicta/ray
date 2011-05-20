#ifndef _GRID_H_
#define _GRID_H_

#include "Math/Ray.h"
#include "Geometry/GeometryObject.h"
#include "Geometry/BBox.h"

struct GridVoxel {
   ~GridVoxel() { objs.clear(); }
   void add(GeometryObject* o) { objs.push_back(o); }
   
   vector<GeometryObject*> objs;
};

class Grid : public GeometryObject {

public:
   Grid() {}
   ~Grid();

   void addObject(GeometryObject* obj);
   void setupCells();
   
   virtual void setHash(Hash* hash) {}
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;

private:
   double calculateNext(double rd, double min, double i, double dt, int n, int& step, int& stop) const;
   bool checkCell(const Ray& ray, GridVoxel* cell, double& tmin, double next, ShadeRecord& sr) const;
   
   vector<GeometryObject*> objs;
   BBox bbox;
   GridVoxel** voxels;
   int nx, ny, nz;
};

#endif
