#include "Mesh.h"
#include "Parser/Hash.h"
#include "Math/Maths.h"
#include "Materials/Matte.h"
#include <assert.h>

#define COUNTER 0
#define CLOCKWISE 1

typedef vector<Vector3D*>::const_iterator VectorIter;
typedef map<unsigned int, SmoothingGroup*>::const_iterator SmoothingGroupIter;
typedef map<int, Vector3D*>::const_iterator SGNormalIter;

Face::Face(Mesh& mesh, int idx1, int idx2, int idx3) : normal(), dpdu(), dpdv(), smoothGroup(0), material(new Matte()), parent(mesh) {
   vertIdxs[0] = idx1;
   vertIdxs[1] = idx2;
   vertIdxs[2] = idx3;

   Point3D* p1 = parent.getPointAt(idx1);
   Point3D* p2 = parent.getPointAt(idx2);
   Point3D* p3 = parent.getPointAt(idx3);

   p1p2 = *p2 - *p1;
   p1p3 = *p3 - *p1;
   normal = p1p2.cross(p1p3);
   if(normal.length() == 0.0) {
      normal.set(0, 1, 0);
   }
   normal.normalize();

}

SmoothingGroup::~SmoothingGroup() {
   for(SGNormalIter it = normals.begin(), end = normals.end(); it != end; it++) {
      delete (*it).second;
   }
   normals.clear();
}

void SmoothingGroup::addFace(Face* f) {
   for(int i = 0; i < 3; i++) {
      if(normals.find(f->vertIdxs[i]) == normals.end()) {
         normals[f->vertIdxs[i]] = new Vector3D();
      }
      *normals[f->vertIdxs[i]] += f->normal;

      if(dpdu.find(f->vertIdxs[i]) == dpdu.end()) {
         dpdu[f->vertIdxs[i]] = new Vector3D();
      }
      *dpdu[f->vertIdxs[i]] += f->dpdu;

      if(dpdv.find(f->vertIdxs[i]) == dpdv.end()) {
         dpdv[f->vertIdxs[i]] = new Vector3D();
      }
      *dpdv[f->vertIdxs[i]] += f->dpdv;
   }
}

void SmoothingGroup::normalize() {
   for(SGNormalIter sit = normals.begin(), end = normals.end(); sit != end; sit++) {
      (*sit).second->normalize();
   }
   for(SGNormalIter sit = dpdu.begin(), end = dpdu.end(); sit != end; sit++) {
      (*sit).second->normalize();
   }
   for(SGNormalIter sit = dpdv.begin(), end = dpdv.end(); sit != end; sit++) {
      (*sit).second->normalize();
   }
}

Vector3D SmoothingGroup::interpolateNormal(Face* face, const double beta, const double gamma) {
   Vector3D normal(*normals[face->vertIdxs[0]] * (1.0 - beta - gamma)
                 + *normals[face->vertIdxs[1]] * beta
                 + *normals[face->vertIdxs[2]] * gamma);
   normal.normalize();
	return normal;
}

Vector3D SmoothingGroup::interpolateDPDU(Face* face, const double beta, const double gamma) {
   Vector3D normal(*dpdu[face->vertIdxs[0]] * (1.0 - beta - gamma)
                 + *dpdu[face->vertIdxs[1]] * beta
                 + *dpdu[face->vertIdxs[2]] * gamma);
   normal.normalize();
	return normal;
}

Vector3D SmoothingGroup::interpolateDPDV(Face* face, const double beta, const double gamma) {
   Vector3D normal(*dpdv[face->vertIdxs[0]] * (1.0 - beta - gamma)
                 + *dpdv[face->vertIdxs[1]] * beta
                 + *dpdv[face->vertIdxs[2]] * gamma);
   normal.normalize();
	return normal;
}


Mesh::Mesh() : GeometryObject(), numCells(0) {
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

void Mesh::addFace(int idx1, int idx2, int idx3) {
   Face* f = new Face(*this, idx1, idx2, idx3);
   computePartialDerivitives(f);
   faces.push_back(f);
}

void Mesh::calculateNormals() {
   if(!smoothingGroups.empty()) {
      for(SmoothingGroupIter it = smoothingGroups.begin(), end = smoothingGroups.end(); it != end; ++it) {
         (*it).second->normalize();
      }
   }
   else {
      normals.reserve(points.size());
      for(unsigned int i = 0; i < points.size(); i++) {
         normals.push_back(new Vector3D());
      }

      for(FaceIter fi = faces.begin(), end = faces.end(); fi != end; ++fi) {
         *normals[(*fi)->vertIdxs[0]] += (*fi)->normal;
         *normals[(*fi)->vertIdxs[1]] += (*fi)->normal;
         *normals[(*fi)->vertIdxs[2]] += (*fi)->normal;
      }

      for(VectorIter it = normals.begin(), end = normals.end(); it != end; ++it) {
         if((*it)->x == 0.0 && (*it)->y == 0.0 && (*it)->z == 0.0) {
            (*it)->y  = 1.0;
         }
         (*it)->normalize();
      }
   }
}

void Mesh::setHash(Hash* hash) {
   if(hash->contains("material")) {
      setupMaterial(hash->getValue("material")->getHash());
   }

   if(hash->contains("verticies")) {
      Array* verts = hash->getValue("verticies")->getArray();
      for(unsigned int i = 0; i < verts->size(); i++) {
         Array* vert = verts->at(i)->getArray();
         addPoint(new Point3D(vert->at(0)->getDouble(), vert->at(1)->getDouble(), vert->at(2)->getDouble()));
      }
   }

   if(hash->contains("faces")) {
      Array* faces = hash->getValue("faces")->getArray();
      for(unsigned int i = 0; i < faces->size(); i++) {
         Array* f = faces->at(i)->getArray();
         addFace(f->at(0)->getInteger(), f->at(1)->getInteger(), f->at(2)->getInteger());
      }

      calculateNormals();
   }
}

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

   const Point3D& p = bbox.contains(ray.origin) ? ray.origin : ray(t0);

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

   double t = 0;
   // Traverse the grid
   while(true) {
      int idx = ix + nx * iy + nx * ny * iz;
//      assert(idx < numCells);
      Voxel* cell = voxels[idx];

      if (tx_next < ty_next && tx_next < tz_next) {
         t = tx_next;
         if(checkCell(ray, cell, t, sr)) { tmin = t; return true; }

         tx_next += dtx;
         ix += ix_step;
         if (ix == ix_stop) return false;
      }
      else if (ty_next < tz_next) {
         t = ty_next;
         if(checkCell(ray, cell, t, sr)) { tmin = t; return true; }

         ty_next += dty;
         iy += iy_step;
         if (iy == iy_stop) return false;
      }
      else {
         t = tz_next;
         if(checkCell(ray, cell, t, sr)) { tmin = t; return true; }

         tz_next += dtz;
         iz += iz_step;
         if (iz == iz_stop) return false;
      }
   }
}

bool Mesh::checkCell(const Ray& ray, Voxel* cell, double& tmin, ShadeRecord& sr) const {
   if(cell == NULL) return false;

   bool hit = false;

   double t;
   for(FaceIter it = cell->faces.begin(), end = cell->faces.end(); it != end; it++) {
      if((t = hitFace(*it, ray, sr)) && t < tmin) {
//      if(hitFace(*it, ray, t, sr) && t < tmin) {
         tmin = t;
         assert((*it)->material.get() != NULL);
         material = (*it)->material;
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

double Mesh::hitFace(Face* face, const Ray& ray, ShadeRecord& sr) const {
   const Vector3D& s1 = ray.direction.cross(face->p1p3);
   double div = s1.dot(face->p1p2);
   if(div < epsilon) {
      return 0;
   }
   float invDiv = 1.0 / div;

   const Vector3D& s = ray.origin - *points[face->vertIdxs[0]];
   const Vector3D& s2 = s.cross(face->p1p2);
   double b1 = s1.dot(s) * invDiv;
   double b2 = s2.dot(ray.direction) * invDiv;

   if(b1 < epsilon || b2 < epsilon || (b1 + b2) > 1.0) {
      return 0;
   }

   double t = s2.dot(face->p1p3) * invDiv;
   if(t < epsilon) {
      return 0;
   }

   if(!smoothingGroups.empty()) {
      if(face->smoothGroup == 0) {
         sr.normal = face->normal;
         sr.dpdu = face->dpdu;
         sr.dpdv = face->dpdv;
      }
      else {
         Vector3D n;
         Vector3D dpdu;
         Vector3D dpdv;
         for(int i = 0; i < 32; i++) {
            int mask = (int) pow(2, i);
            if(mask & face->smoothGroup) {
               n += (*smoothingGroups.find(i)).second->interpolateNormal(face, b1, b2);
               dpdu += (*smoothingGroups.find(i)).second->interpolateDPDU(face, b1, b2);
               dpdv += (*smoothingGroups.find(i)).second->interpolateDPDV(face, b1, b2);
            }
         }
         n.normalize();
         sr.normal = n;

         dpdu.normalize();
         sr.dpdu = dpdu;

         dpdv.normalize();
         sr.dpdv = dpdv;
      }
   }
   else {
      sr.normal = face->normal; // interpolateNormal(face, beta, gamma);
   }
   sr.localHitPoint = ray(t);

   if(textureCoords.size() != points.size()) {
      if(textureCoords.size() > 0) {
         printf("%lu %lu\n", textureCoords.size(), points.size());
         exit(1);
      }
      sr.tu = sr.tv = 0;
   }
   else {
      double b0 = 1.0 - b1 - b2;
      sr.tu = b0 * textureCoords[face->vertIdxs[0]]->x + b1 * textureCoords[face->vertIdxs[1]]->x + b2 * textureCoords[face->vertIdxs[2]]->x;
      sr.tv = b0 * textureCoords[face->vertIdxs[0]]->y + b1 * textureCoords[face->vertIdxs[1]]->y + b2 * textureCoords[face->vertIdxs[2]]->y;
      sr.tu = normalize(sr.tu);
      sr.tv = 1.0 - normalize(sr.tv);
   }

   return t;
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

void Mesh::computePartialDerivitives(Face* face) const {
   double uvs[3][2];
   getUVs(uvs, face);

   Vector3D dp1 = *points[face->vertIdxs[0]] - *points[face->vertIdxs[2]];
   Vector3D dp2 = *points[face->vertIdxs[1]] - *points[face->vertIdxs[2]];

   // Compute deltas
   double du1 = uvs[0][0] - uvs[2][0];
   double du2 = uvs[1][0] - uvs[2][0];
   double dv1 = uvs[0][1] - uvs[2][1];
   double dv2 = uvs[1][1] - uvs[2][1];

   double determinant = du1 * dv2 - dv1 * du2;
   if(determinant == 0.0) {
      coordinateSystem(face->p1p3.cross(face->p1p2).normalize(), &face->dpdu, &face->dpdv);
   }
   else {
      face->dpdu = (dp1 * dv2 - dp2 * dv1) / determinant;
      face->dpdu.normalize();
      face->dpdv = (dp1 * -du2 + dp2 * du1) / determinant;
      face->dpdv.normalize();
   }
}

void Mesh::getUVs(double uv[3][2], Face* face) const {
   if(textureCoords.empty()) {
      uv[0][0] = 0.0;
      uv[0][1] = 0.0;
      uv[1][0] = 1.0;
      uv[1][1] = 0.0;
      uv[2][0] = 1.0;
      uv[2][1] = 1.0;
   }
   else {
      for(int i = 0; i < 3; i++) {
         unsigned int idx = face->vertIdxs[i];
         assert(idx < textureCoords.size());
         uv[i][0] = textureCoords[idx]->x;
         uv[i][1] = textureCoords[idx]->y;
      }
   }
}

void Mesh::setupCells() {
   double root = 3.0 * pow(faces.size(), 1.0 / 3.0);
   double voxelsPerUnit = root / bbox.maxExtent();

   nx = (int) clamp(round(bbox.wx * voxelsPerUnit), 0, 32) + 1;
   ny = (int) clamp(round(bbox.wy * voxelsPerUnit), 0, 32) + 1;
   nz = (int) clamp(round(bbox.wz * voxelsPerUnit), 0, 32) + 1;

   numCells = nx * ny * nz;
   voxels = new Voxel*[numCells];
   memset(voxels, 0, sizeof(Voxel*) * numCells);

   for(FaceIter it = faces.begin(), end = faces.end(); it != end; ++it) {
      BBox fbox;
      fbox.expand(*points[(*it)->vertIdxs[0]]);
      fbox.expand(*points[(*it)->vertIdxs[1]]);
      fbox.expand(*points[(*it)->vertIdxs[2]]);

      int ixmin = (int) clamp((fbox.x0 - bbox.x0) * nx / bbox.wx, 0, nx - 1);
      int iymin = (int) clamp((fbox.y0 - bbox.y0) * ny / bbox.wy, 0, ny - 1);
      int izmin = (int) clamp((fbox.z0 - bbox.z0) * nz / bbox.wz, 0, nz - 1);
      int ixmax = (int) clamp((fbox.x1 - bbox.x0) * nx / bbox.wx, 0, nx - 1);
      int iymax = (int) clamp((fbox.y1 - bbox.y0) * ny / bbox.wy, 0, ny - 1);
      int izmax = (int) clamp((fbox.z1 - bbox.z0) * nz / bbox.wz, 0, nz - 1);

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

void Mesh::setFaceMaterial(int idx, shared_ptr<Material> material) {
   faces[idx]->material = material;
}

