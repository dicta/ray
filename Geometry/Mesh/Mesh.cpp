#include "Mesh.h"
#include "Parser/Hash.h"
#include "Math/Maths.h"

#define COUNTER 0
#define CLOCKWISE 1

typedef vector<Vector3D*>::const_iterator VectorIter;

Mesh::Mesh() : GeometryObject(), bbox() {
   doDelete = false;
   name = "";
}

Mesh::~Mesh() {
   for(unsigned int i = 0; i < points.size(); i++) {
      delete points[i];
   }
   points.clear();
   
   for(unsigned int i = 0; i < normals.size(); i++) {
      delete normals[i];
   }
   normals.clear();
   
   for(unsigned int i = 0; i < faces.size(); i++) {
      delete faces[i];
   }
   faces.clear();
}

int Mesh::addPoint(Point3D* p) {
   points.push_back(p);
   bbox.expand(*p);
   return points.size() - 1;
}

void Mesh::addFace(Face* f) {
   Point3D* p1 = points[f->vertIdxs[0]];
   Point3D* p2 = points[f->vertIdxs[1]];
   Point3D* p3 = points[f->vertIdxs[2]];
   
//   double x = (p1->y - p2->y) * (p1->z + p2->z) + (p2->y - p3->y) * (p2->z + p3->z) + (p3->y - p1->y) * (p3->z + p1->z);
//   double y = (p1->z - p2->z) * (p1->x + p2->x) + (p2->z - p3->z) * (p2->x + p3->x) + (p3->z - p1->z) * (p3->x + p1->x);
//   double z = (p1->x - p2->x) * (p1->y + p2->y) + (p2->x - p3->x) * (p2->y + p3->y) + (p3->x - p1->x) * (p3->y + p1->y);

//   f->normal.set(x, y, z);
   f->normal = ((*points[f->vertIdxs[1]]) - (*points[f->vertIdxs[0]])).cross((*points[f->vertIdxs[2]]) - (*points[f->vertIdxs[0]]));
   f->normal.normalize();
   
   f->bbox.expand(*p1);
   f->bbox.expand(*p2);
   f->bbox.expand(*p3);
   
   faces.push_back(f);
}

void Mesh::calculateNormals() {
   normals.reserve(points.size());
   for(unsigned int i = 0; i < points.size(); i++) {
      normals.push_back(new Vector3D());
   }
   
   for(FaceIter fi = faces.begin(); fi != faces.end(); fi++) {
      int idx1 = (*fi)->vertIdxs[0];
      int idx2 = (*fi)->vertIdxs[1];
      int idx3 = (*fi)->vertIdxs[2];

      *normals[idx1] += (*fi)->normal;
      *normals[idx2] += (*fi)->normal;
      *normals[idx3] += (*fi)->normal;
   }
   
   for(VectorIter it = normals.begin(); it != normals.end(); it++) {
      if((*it)->x == 0.0 && (*it)->y == 0.0 && (*it)->z == 0.0) {
         (*it)->y  = 1.0;
      }
      (*it)->normalize();
   }
}

void Mesh::setHash(Hash* hash) {
   if(hash->contains("material")) {
      setupMaterial(hash->getValue("material")->getHash());
   }
   
   if(hash->contains("verticies")) {
      Array* verts = hash->getValue("verticies")->getArray();
      for(int i = 0; i < verts->size(); i++) {
         Array* vert = verts->at(i)->getArray();
         addPoint(new Point3D(vert->at(0)->getDouble(), vert->at(1)->getDouble(), vert->at(2)->getDouble()));
      }
   }
   
   if(hash->contains("faces")) {
      Array* faces = hash->getValue("faces")->getArray();
      for(int i = 0; i < faces->size(); i++) {
         Array* f = faces->at(i)->getArray();
         addFace(new Face(f->at(0)->getInteger(), f->at(1)->getInteger(), f->at(2)->getInteger()));
      }
      
      calculateNormals();
   }
}
/*
bool Mesh::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   if(!bbox.hit(ray)) {
      return false;
   }

   bool hit = false;
   double t;
   tmin = 1.7 * pow(10.0, 308.0);

   for(FaceIter it = faces.begin(); it != faces.end(); it++) {
      if(hitFace(*it, ray, t, sr) && (t < tmin)) {
         hit = true;
         tmin = t;
      }
   }

   return hit;
}
*/

bool Mesh::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
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
      Voxel* cell = voxels[ix + nx * iy + nx * ny * iz];

      if (tx_next < ty_next && tx_next < tz_next) {
         tmin = tx_next;
         if(checkCell(ray, cell, tmin, sr)) return true;

         tx_next += dtx;
         ix += ix_step;
         if (ix == ix_stop) return false;
      }
      else if (ty_next < tz_next) {
         tmin = ty_next;
         if(checkCell(ray, cell, tmin, sr)) return true;

         ty_next += dty;
         iy += iy_step;
         if (iy == iy_stop) return false;
      }
      else {
         tmin = tz_next;
         if(checkCell(ray, cell, tmin, sr)) return true;

         tz_next += dtz;
         iz += iz_step;
         if (iz == iz_stop) return false;
      }
   }
}

bool Mesh::checkCell(const Ray& ray, Voxel* cell, double& tmin, ShadeRecord& sr) const {
   if(cell == NULL) return false;

   double t;
   bool hit = false;

   for(FaceIter it = cell->faces.begin(); it != cell->faces.end(); it++) {
      if(hitFace(*it, ray, t, sr) && t < tmin) {
         tmin = t;
         hit = true;
      }
   }

   return hit;
}

double Mesh::calculateNext(double rd, double min, double i, double dt, int n, int& step, int& stop) const {
   double next;

   if (fabs(rd) < epsilon) {
      next = HUGE_VALUE;
      step = -1;
      stop = -1;
   }
   else if (rd > 0) {
      next = min + (i + 1) * dt;
      step = 1;
      stop = n;
   }
   else {
      next = min + (n - i) * dt;
      step = -1;
      stop = -1;
   }
   
   return next;
}

bool Mesh::hitFace(Face* face, const Ray& ray, double& tmin, ShadeRecord& sr) const {
   Point3D* p1 = points[face->vertIdxs[0]];
   Point3D* p2 = points[face->vertIdxs[1]];
   Point3D* p3 = points[face->vertIdxs[2]];

   Vector3D e1 = *p2 - *p1;
   Vector3D e2 = *p3 - *p1;
   Vector3D s1 = ray.direction.cross(e2);
   double div = s1.dot(e1);
   if(div < epsilon) {
      return false;
   }
   float invDiv = 1.0 / div;
   
   Vector3D s = ray.origin - *p1;
   double b1 = s1.dot(s) * invDiv;
   if(b1 < epsilon || b1 > 1.0) {
      return false;
   }
   
   Vector3D s2 = s.cross(e1);
   double b2 = s2.dot(ray.direction) * invDiv;
   if(b2 < epsilon || (b1 + b2) > 1.0) {
      return false;
   }
   
   double t = s2.dot(e2) * invDiv;
   if(t < epsilon) {
      return false;
   }

   tmin = t;
   sr.normal = face->normal; // interpolateNormal(face, beta, gamma);
   sr.localHitPoint = ray.origin + ray.direction * t;

   return true;
}

bool Mesh::shadowHit(const Ray& ray, double& tmin) const {
   ShadeRecord sr;
   return hit(ray, tmin, sr);
}

Vector3D Mesh::interpolateNormal(Face* face, const double beta, const double gamma) const {
   Vector3D normal(*normals[face->vertIdxs[0]] * (1.0 - beta - gamma)
                 + *normals[face->vertIdxs[1]] * beta 
                 + *normals[face->vertIdxs[2]] * gamma);
   normal.normalize();
	return normal;
}

void Mesh::setupCells() {
   double root = 3.0 * pow(faces.size(), 1.0 / 3.0);
   double voxelsPerUnit = root / bbox.maxExtent();
   nx = (int) clamp(round(bbox.wx * voxelsPerUnit), 0, 64) + 1;
   ny = (int) clamp(round(bbox.wy * voxelsPerUnit), 0, 64) + 1;
   nz = (int) clamp(round(bbox.wz * voxelsPerUnit), 0, 64) + 1;

   int numCells = nx * ny * nz;
   voxels = new Voxel*[numCells];
   memset(voxels, 0, sizeof(Voxel*) * numCells);

   for(FaceIter it = faces.begin(); it != faces.end(); it++) {
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
                  voxels[index] = new Voxel();
               }
               voxels[index]->add(*it);
            }
         }
      }
   }
}
