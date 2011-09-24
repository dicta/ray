#include "Grid.h"
#include "Math/Maths.h"

typedef vector<GeometryObject*>::const_iterator CellIter;

GridVoxel::GridVoxel() : objs() {
}

GridVoxel::~GridVoxel() {
   objs.clear();
}

void GridVoxel::add(GeometryObject* o) {
   objs.push_back(o);
}

Grid::Grid() : Storage(), numCells(0), voxels(NULL), nx(0), ny(0), nz(0), maxCells(64) {
}

Grid::~Grid() {
//   cleanup();
}

void Grid::cleanup() {
   if(numCells > 0) {
      for(int i = 0; i < numCells; i++) {
         if(voxels[i] != NULL) {
            delete voxels[i];
         }
      }
      delete[] voxels;
      voxels = NULL;
   }
}

void Grid::setup() {
//   cleanup();
   double root = 3.0 * pow(objs.size(), 1.0 / 3.0);
   double voxelsPerUnit = root / bbox.maxExtent();
//   int maxCells = int(8 + 1.3f * Log2Int(float(objs.size())));
   printf("grid.maxCells = %d\n", maxCells);
   nx = (int) clamp(round(bbox.wx * voxelsPerUnit), 0, maxCells) + 1;
   ny = (int) clamp(round(bbox.wy * voxelsPerUnit), 0, maxCells) + 1;
   nz = (int) clamp(round(bbox.wz * voxelsPerUnit), 0, maxCells) + 1;

   numCells = nx * ny * nz;
   voxels = new GridVoxel*[numCells];
   memset(voxels, 0, sizeof(GridVoxel*) * numCells);

   for(GeomIter it = objs.begin(); it != objs.end(); it++) {
      int ixmin = (int) clamp(((*it)->bbox.x0 - bbox.x0) * nx / bbox.wx, 0, nx - 1);
      int iymin = (int) clamp(((*it)->bbox.y0 - bbox.y0) * ny / bbox.wy, 0, ny - 1);
      int izmin = (int) clamp(((*it)->bbox.z0 - bbox.z0) * nz / bbox.wz, 0, nz - 1);
      int ixmax = (int) clamp(((*it)->bbox.x1 - bbox.x0) * nx / bbox.wx, 0, nx - 1);
      int iymax = (int) clamp(((*it)->bbox.y1 - bbox.y0) * ny / bbox.wy, 0, ny - 1);
      int izmax = (int) clamp(((*it)->bbox.z1 - bbox.z0) * nz / bbox.wz, 0, nz - 1);

      // add the object to the cells
      for(int iz = izmin; iz <= izmax; iz++) {
         for(int iy = iymin; iy <= iymax; iy++) {
            for(int ix = ixmin; ix <= ixmax; ix++) {
               int index = ix + nx * iy + nx * ny * iz;
               if(voxels[index] == NULL) {
                  voxels[index] = new GridVoxel();
               }
               voxels[index]->add(*it);
            }
         }
      }
   }
}

bool Grid::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
	// the following code includes modifications from Shirley and Morley (2003)

   double tx_min = (bbox.x0 - ray.origin.x) / ray.direction.x;
   double tx_max = (bbox.x1 - ray.origin.x) / ray.direction.x;
   if(ray.direction.x < 0) swap(tx_min, tx_max);

   double ty_min = (bbox.y0 - ray.origin.y) / ray.direction.y;
   double ty_max = (bbox.y1 - ray.origin.y) / ray.direction.y;
   if(ray.direction.y < 0) swap(ty_min, ty_max);

   double tz_min = (bbox.z0 - ray.origin.z) / ray.direction.z;
   double tz_max = (bbox.z1 - ray.origin.z) / ray.direction.z;
   if(ray.direction.z < 0) swap(tz_min, tz_max);

	double t0 = max(max(tx_min, ty_min), tz_min);
	double t1 = min(min(tx_max, ty_max), tz_max);

	if (t0 > t1) return(false);

   Point3D p = ray.origin;
   if(!bbox.contains(ray.origin)) {
      p = ray(t0);
   }

   int ix = (int) clamp((p.x - bbox.x0) * nx / (bbox.wx), 0, nx - 1);
   int iy = (int) clamp((p.y - bbox.y0) * ny / (bbox.wy), 0, ny - 1);
   int iz = (int) clamp((p.z - bbox.z0) * nz / (bbox.wz), 0, nz - 1);

   // ray parameter increments per cell in the x, y, and z directions

   double dtx = (tx_max - tx_min) / nx;
   double dty = (ty_max - ty_min) / ny;
   double dtz = (tz_max - tz_min) / nz;

   int ix_step, iy_step, iz_step;
   int ix_stop, iy_stop, iz_stop;

   double tx_next = calculateNext(ray.direction.x, tx_min, ix, dtx, nx, ix_step, ix_stop);
   double ty_next = calculateNext(ray.direction.y, ty_min, iy, dty, ny, iy_step, iy_stop);
   double tz_next = calculateNext(ray.direction.z, tz_min, iz, dtz, nz, iz_step, iz_stop);

   // Traverse the grid
   while(true) {
      GridVoxel* cell = voxels[ix + nx * iy + nx * ny * iz];

      if (tx_next < ty_next && tx_next < tz_next) {
//         tmin = tx_next;
         if(checkCell(ray, cell, tmin, sr)) return true;

         tx_next += dtx;
         ix += ix_step;
         if (ix == ix_stop) return false;
      }
      else if (ty_next < tz_next) {
//         tmin = ty_next;
         if(checkCell(ray, cell, tmin, sr)) return true;

         ty_next += dty;
         iy += iy_step;
         if (iy == iy_stop) return false;
      }
      else {
//         tmin = tz_next;
         if(checkCell(ray, cell, tmin, sr)) return true;

         tz_next += dtz;
         iz += iz_step;
         if (iz == iz_stop) return false;
      }
   }
}

bool Grid::shadowHit(const Ray& ray, double& tmin) const {
   double tx_min = (bbox.x0 - ray.origin.x) / ray.direction.x;
   double tx_max = (bbox.x1 - ray.origin.x) / ray.direction.x;
   if(ray.direction.x < 0) swap(tx_min, tx_max);

   double ty_min = (bbox.y0 - ray.origin.y) / ray.direction.y;
   double ty_max = (bbox.y1 - ray.origin.y) / ray.direction.y;
   if(ray.direction.y < 0) swap(ty_min, ty_max);

   double tz_min = (bbox.z0 - ray.origin.z) / ray.direction.z;
   double tz_max = (bbox.z1 - ray.origin.z) / ray.direction.z;
   if(ray.direction.z < 0) swap(tz_min, tz_max);

   double t0 = max(max(tx_min, ty_min), tz_min);
   double t1 = min(min(tx_max, ty_max), tz_max);

   if (t0 > t1) return(false);

   Point3D p = ray.origin;
   if(!bbox.contains(ray.origin)) {
      p = ray(t0);
   }

   int ix = (int) clamp((p.x - bbox.x0) * nx / (bbox.wx), 0, nx - 1);
   int iy = (int) clamp((p.y - bbox.y0) * ny / (bbox.wy), 0, ny - 1);
   int iz = (int) clamp((p.z - bbox.z0) * nz / (bbox.wz), 0, nz - 1);

   // ray parameter increments per cell in the x, y, and z directions

   double dtx = (tx_max - tx_min) / nx;
   double dty = (ty_max - ty_min) / ny;
   double dtz = (tz_max - tz_min) / nz;

   int ix_step, iy_step, iz_step;
   int ix_stop, iy_stop, iz_stop;

   double tx_next = calculateNext(ray.direction.x, tx_min, ix, dtx, nx, ix_step, ix_stop);
   double ty_next = calculateNext(ray.direction.y, ty_min, iy, dty, ny, iy_step, iy_stop);
   double tz_next = calculateNext(ray.direction.z, tz_min, iz, dtz, nz, iz_step, iz_stop);

   // Traverse the grid
   while(true) {
      GridVoxel* cell = voxels[ix + nx * iy + nx * ny * iz];

      if (tx_next < ty_next && tx_next < tz_next) {
         if(checkCellShadow(ray, cell, tmin)) return true;

         tx_next += dtx;
         ix += ix_step;
         if (ix == ix_stop) return false;
      }
      else if (ty_next < tz_next) {
         if(checkCellShadow(ray, cell, tmin)) return true;

         ty_next += dty;
         iy += iy_step;
         if (iy == iy_stop) return false;
      }
      else {
         if(checkCellShadow(ray, cell, tmin)) return true;

         tz_next += dtz;
         iz += iz_step;
         if (iz == iz_stop) return false;
      }
   }
}

double Grid::calculateNext(double rd, double min, double i, double dt, int n, int& step, int& stop) const {
   double next;

   if (rd > 0) {
      next = min + (i + 1) * dt;
      step = 1;
      stop = n;
   }
   else {
      next = min + (n - i) * dt;
      step = -1;
      stop = -1;
   }

   if (rd == 0.0) {
      next = HUGE_VALUE;
      step = -1;
      stop = -1;
   }

   return next;
}

bool Grid::checkCell(const Ray& ray, GridVoxel* cell, double& tmin, ShadeRecord& sr) const {
   if(cell == NULL) return false;

   bool hit = false;
   double tcheck = HUGE_VALUE;
   Vector3D normal;
   Point3D hitPoint;
   Point3D localHitPoint;
   shared_ptr<Material> mat;

   for(CellIter it = cell->objs.begin(); it != cell->objs.end(); it++) {
      if((*it)->hit(ray, tmin, sr) && tmin < tcheck) {
         mat = (*it)->getMaterial();
         localHitPoint = sr.localHitPoint;
         normal = sr.normal;
         hitPoint = ray(tmin);
         hit = true;
         tcheck = tmin;
      }
   }

   if(hit) {
      sr.localHitPoint = localHitPoint;
      sr.hitPoint = hitPoint;
      sr.normal = normal;
      sr.material = mat;
   }
   return hit;
}

bool Grid::checkCellShadow(const Ray& ray, GridVoxel* cell, double& tmin) const {
   if(cell == NULL) return false;

   bool hit = false;
   double tcheck = HUGE_VALUE;

   for(CellIter it = cell->objs.begin(); it != cell->objs.end(); it++) {
      if(!(*it)->ignoreShadow && (*it)->shadowHit(ray, tmin) && tmin < tcheck) {
         hit = true;
         tcheck = tmin;
      }
   }
   return hit;
}
