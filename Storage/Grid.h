#ifndef _GRID_H_
#define _GRID_H_

#include "Math/Ray.h"
#include "Geometry/GeometryObject.h"
#include <list>
#include <vector>

typedef list<GeometryObject*>::const_iterator GeomIter;

class GridVoxel {

public:
   GridVoxel();
   ~GridVoxel();

   void add(GeometryObject* o);
   vector<GeometryObject*> objs;
};

class Grid : public GeometryObject {

public:
   Grid();
   ~Grid();

   void addObject(GeometryObject* obj);
   void removeObject(GeometryObject* obj) { objs.remove(obj); }
   void setupCells();

   virtual void setHash(Hash* hash) {}
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;

   void setMaxSize(int size) { maxSize = size; }

private:
   void cleanup();
   double calculateNext(double rd, double min, double i, double dt, int n, int& step, int& stop) const;
   bool checkCell(const Ray& ray, GridVoxel* cell, double& tmin, ShadeRecord& sr) const;
   bool checkCellShadow(const Ray& ray, GridVoxel* cell, double& tmin) const;

   int numCells;
   list<GeometryObject*> objs;
   GridVoxel** voxels;
   int nx, ny, nz;
   int maxSize;
};

#endif
