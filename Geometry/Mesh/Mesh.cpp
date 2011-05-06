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
   int dir = determinant(f);
   if(dir == COUNTER) {
//      f->normal = ((*points[idx1]) - (*points[idx0])).cross((*points[idx2]) - (*points[idx0]));
   }
   else {
//      f->normal = ((*points[idx2]) - (*points[idx0])).cross((*points[idx1]) - (*points[idx0]));
//      int t = f->vertIdxs[0];
//      f->vertIdxs[0] = f->vertIdxs[1];
//      f->vertIdxs[1] = t;
   }
   
   Point3D* p1 = points[f->vertIdxs[0]];
   Point3D* p2 = points[f->vertIdxs[1]];
   Point3D* p3 = points[f->vertIdxs[2]];
   
   double x = (p1->y - p2->y) * (p1->z + p2->z) + (p2->y - p3->y) * (p2->z + p3->z) + (p3->y - p1->y) * (p3->z + p1->z);
   double y = (p1->z - p2->z) * (p1->x + p2->x) + (p2->z - p3->z) * (p2->x + p3->x) + (p3->z - p1->z) * (p3->x + p1->x);
   double z = (p1->x - p2->x) * (p1->y + p2->y) + (p2->x - p3->x) * (p2->y + p3->y) + (p3->x - p1->x) * (p3->y + p1->y);

   f->normal.set(x, y, z);
//   f->normal = ((*points[idx1]) - (*points[idx0])).cross((*points[idx2]) - (*points[idx0]));
   f->normal.normalize();
   
   f->bbox.expand(*p1);
   f->bbox.expand(*p2);
   f->bbox.expand(*p3);
   
   faces.push_back(f);
}

int Mesh::determinant(Face* f) {
	double x1 = points[f->vertIdxs[0]]->x;
	double y1 = points[f->vertIdxs[0]]->y;
	double x2 = points[f->vertIdxs[1]]->x;
	double y2 = points[f->vertIdxs[1]]->y;
	double x3 = points[f->vertIdxs[2]]->x;
	double y3 = points[f->vertIdxs[2]]->y;

	double determ = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
	if (determ >= 0.0) {
//    	printf("counter\n");
    	return COUNTER;
   }
	else {
//    	printf("clockwise\n");
      return CLOCKWISE;
   }
} /* End of determinant */

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
/*
bool Mesh::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   double kHugeValue	= 1.0E10;

   double x0 = bbox.x0;
   double y0 = bbox.y0;
   double z0 = bbox.z0;
   double x1 = bbox.x1;
   double y1 = bbox.y1;
   double z1 = bbox.z1;
	
   double tx_min, ty_min, tz_min;
   double tx_max, ty_max, tz_max; 
	
	// the following code includes modifications from Shirley and Morley (2003)
	
   double a = 1.0 / ray.direction.x;
   if (a >= 0) {
      tx_min = (x0 - ray.origin.x) * a;
      tx_max = (x1 - ray.origin.x) * a;
   }
   else {
      tx_min = (x1 - ray.origin.x) * a;
      tx_max = (x0 - ray.origin.x) * a;
   }
	
	double b = 1.0 / ray.direction.y;
	if (b >= 0) {
		ty_min = (y0 - ray.origin.y) * b;
		ty_max = (y1 - ray.origin.y) * b;
	}
	else {
		ty_min = (y1 - ray.origin.y) * b;
		ty_max = (y0 - ray.origin.y) * b;
	}
	
	double c = 1.0 / ray.direction.z;
	if (c >= 0) {
		tz_min = (z0 - ray.origin.z) * c;
		tz_max = (z1 - ray.origin.z) * c;
	}
	else {
		tz_min = (z1 - ray.origin.z) * c;
		tz_max = (z0 - ray.origin.z) * c;
	}
	
	double t0, t1;
	
	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;
   
	if (tz_min > t0)
		t0 = tz_min;
   
	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;
   
	if (tz_max < t1)
		t1 = tz_max;
   
	if (t0 > t1)
		return(false);   
   
   int ix, iy, iz;
   
   if(bbox.contains(ray.origin)) {
      // Ray starts inside grid
      ix = (int) clamp((ray.origin.x - bbox.x0) * nx / (bbox.x1 - bbox.x0), 0, nx - 1);
      iy = (int) clamp((ray.origin.y - bbox.y0) * ny / (bbox.y1 - bbox.y0), 0, ny - 1);
      iz = (int) clamp((ray.origin.z - bbox.z0) * nz / (bbox.z1 - bbox.z0), 0, nz - 1);
   }
   else {
      Point3D p = ray.origin + ray.direction * t0;  // initial hit point with grid's bounding box
      ix = (int) clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
      iy = (int) clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
      iz = (int) clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
   }
   
   // ray parameter increments per cell in the x, y, and z directions

   double dtx = (tx_max - tx_min) / nx;
   double dty = (ty_max - ty_min) / ny;
   double dtz = (tz_max - tz_min) / nz;

   double   tx_next, ty_next, tz_next;
   int   ix_step, iy_step, iz_step;
   int   ix_stop, iy_stop, iz_stop;

   if (ray.direction.x > 0) {
      tx_next = tx_min + (ix + 1) * dtx;
      ix_step = +1;
      ix_stop = nx;
   }
   else {
      tx_next = tx_min + (nx - ix) * dtx;
      ix_step = -1;
      ix_stop = -1;
   }

   if (ray.direction.x == 0.0) {
      tx_next = kHugeValue;
      ix_step = -1;
      ix_stop = -1;
   }


   if (ray.direction.y > 0) {
      ty_next = ty_min + (iy + 1) * dty;
      iy_step = +1;
      iy_stop = ny;
   }
   else {
      ty_next = ty_min + (ny - iy) * dty;
      iy_step = -1;
      iy_stop = -1;
   }

   if (ray.direction.y == 0.0) {
      ty_next = kHugeValue;
      iy_step = -1;
      iy_stop = -1;
   }

   if (ray.direction.z > 0) {
      tz_next = tz_min + (iz + 1) * dtz;
      iz_step = +1;
      iz_stop = nz;
   }
   else {
      tz_next = tz_min + (nz - iz) * dtz;
      iz_step = -1;
      iz_stop = -1;
   }

   if (ray.direction.z == 0.0) {
      tz_next = kHugeValue;
      iz_step = -1;
      iz_stop = -1;
   }

   // Traverse the grid
   while(true) {
      vector<Face*> cell = cells[ix + nx * iy + nx * ny * iz];
      if (tx_next < ty_next && tx_next < tz_next) {
         double t;
         bool hit = false;
         tmin = tx_next;
         for(FaceIter it = cell.begin(); it != cell.end(); it++) {
            if(hitFace(*it, ray, t, sr) && t < tmin) {
               tmin = t;
               hit = true;
            }
         }
         if(hit) return true;

         tx_next += dtx;
         ix += ix_step;
         if (ix == ix_stop) return false;
      }
      else {
         if (ty_next < tz_next) {
            double t;
            bool hit = false;
            tmin = ty_next;
            for(FaceIter it = cell.begin(); it != cell.end(); it++) {
               if(hitFace(*it, ray, t, sr) && t < tmin) {
                  tmin = t;
                  hit = true;
               }
            }
            if(hit) return true;

            ty_next += dty;
            iy += iy_step;

            if (iy == iy_stop) return false;
         }
         else {
            double t;
            bool hit = false;
            tmin = tz_next;
            for(FaceIter it = cell.begin(); it != cell.end(); it++) {
               if(hitFace(*it, ray, t, sr) && t < tmin) {
                  tmin = t;
                  hit = true;
               }
            }
            if(hit) return true;

            tz_next += dtz;
            iz += iz_step;

            if (iz == iz_stop) return false;
         }
      }
   }
}
*/
bool Mesh::hitFace(Face* face, const Ray& ray, double& tmin, ShadeRecord& sr) const {
   Point3D* v0 = points[face->vertIdxs[0]];
   Point3D* v1 = points[face->vertIdxs[1]];
   Point3D* v2 = points[face->vertIdxs[2]];
   
   double a = v0->x - v1->x, b = v0->x - v2->x, c = ray.direction.x, d = v0->x - ray.origin.x;
   double e = v0->y - v1->y, f = v0->y - v2->y, g = ray.direction.y, h = v0->y - ray.origin.y;
   double i = v0->z - v1->z, j = v0->z - v2->z, k = ray.direction.z, l = v0->z - ray.origin.z;
   
   double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
   double q = g * i - e * k, s = e * j - f * i;
   double invDenom  = 1.0 / (a * m + b * q + c * s);
   double e1 = d * m - b * n - c * p;
   double beta = e1 * invDenom;
   
   if(beta < 0.0) {
      return false;
   }
   
   double r = e * l - h * i;
   double e2 = a * n + d * q + c * r;
   double gamma = e2 * invDenom;
   
   if(gamma < 0.0) {
      return false;
   }
   
   if(beta + gamma > 1.0) {
      return false;
   }
   
   double e3 = a * p - b * r + d * s;
   double t = e3 * invDenom;
   
   if(t < epsilon) {
      return false;
   }
   
   tmin = t;
   sr.normal = interpolateNormal(face, beta, gamma);
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
   // dimensions of the grid in the x, y, and z directions
   double wx = bbox.x1 - bbox.x0;
   double wy = bbox.y1 - bbox.y0;
   double wz = bbox.z1 - bbox.z0;

   double multiplier = 2.0;
   
   double s = pow(wx * wy * wz / faces.size(), 0.33333333);
   nx = multiplier * wx / s + 1;
   ny = multiplier * wy / s + 1;
   nz = multiplier * wz / s + 1;
   
   int numCells = nx * ny * nz;
   cells.reserve(numCells);
   for(int i = 0; i < numCells; i++) {
      vector<Face*> v;
      cells.push_back(v);
   }

   for(FaceIter it = faces.begin(); it != faces.end(); it++) {
      int ixmin = clamp(((*it)->bbox.x0 - bbox.x0) * nx / (bbox.x1 - bbox.x0), 0, nx - 1);
      int iymin = clamp(((*it)->bbox.y0 - bbox.y0) * ny / (bbox.y1 - bbox.y0), 0, ny - 1);
      int izmin = clamp(((*it)->bbox.z0 - bbox.z0) * nz / (bbox.z1 - bbox.z0), 0, nz - 1);
      int ixmax = clamp(((*it)->bbox.x1 - bbox.x0) * nx / (bbox.x1 - bbox.x0), 0, nx - 1);
      int iymax = clamp(((*it)->bbox.y1 - bbox.y0) * ny / (bbox.y1 - bbox.y0), 0, ny - 1);
      int izmax = clamp(((*it)->bbox.z1 - bbox.z0) * nz / (bbox.z1 - bbox.z0), 0, nz - 1);
      
      // add the object to the cells
      for(int iz = izmin; iz <= izmax; iz++) {
         for(int iy = iymin; iy <= iymax; iy++) {
            for(int ix = ixmin; ix <= ixmax; ix++) {
               int index = ix + nx * iy + nx * ny * iz;
               cells[index].push_back(new Face(0, 1, 2)); // *it);
            }
         }
      }
   }
}
