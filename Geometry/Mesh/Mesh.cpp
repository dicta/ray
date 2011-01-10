#include "Mesh.h"

typedef vector<Vector3D*>::const_iterator VectorIter;

Mesh::Mesh() : GeometryObject(), bbox() {
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
      
      Point3D* p0 = points[idx1];
      Point3D* p1 = points[idx2];
      Point3D* p2 = points[idx3];
      
      Vector3D n(*p1 - *p0);
      n = n.cross(*p2 - *p1);
      n.normalize();
      
      *normals[idx1] += n;
      *normals[idx2] += n;
      *normals[idx3] += n;
   }
   
   for(VectorIter it = normals.begin(); it != normals.end(); it++) {
      (*it)->normalize();
   }
}

void Mesh::setHash(Hash* hash) {
}

bool Mesh::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   if(!bbox.hit(ray)) {
//      return false;
   }

   bool hit = false;
   double t;
   tmin = 1.7 * pow(10.0, 308.0);
   
   for(FaceIter it = faces.begin(); it != faces.end(); it++) {
      if(hitFace(*it, ray, t, sr) && (t < tmin)) {
         hit = true;
         tmin = t;
         sr.localHitPoint = ray.origin + ray.direction * t;
      }
   }

   return hit;
}

bool Mesh::hitFace(Face* face, const Ray& ray, double& tmin, ShadeRecord& sr) const {
   Point3D* v0 = points[face->vertIdxs[0]];
   Point3D* v1 = points[face->vertIdxs[1]];
   Point3D* v2 = points[face->vertIdxs[2]];
   
   double a = v0->x - v1->x, b = v0->x - v2->x, c = ray.direction.x, d = v0->x - ray.origin.x;
   double e = v0->y - v1->y, f = v0->y - v2->y, g = ray.direction.y, h = v0->y - ray.origin.y;
   double i = v0->z - v1->z, j = v0->z - v2->z, k = ray.direction.z, l = v0->z - ray.origin.z;
   
   double m = f * k -g * j, n = h * k -g * l, p = f * l - h * j;
   double q = g * i -e * k, s = e * j - f * i;
   
   double invDenom = 1.0 / (a * m + b * q + c * s);
   
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
   
   sr.normal = interpolateNormal(face, beta, gamma);
   
   return true;
}

bool Mesh::shadowHit(const Ray& ray, double& tmin) const {
   return false;
}

Vector3D Mesh::interpolateNormal(Face* face, const double beta, const double gamma) const {
   Vector3D normal(*normals[face->vertIdxs[0]] * (1.0 - beta - gamma)
                 + *normals[face->vertIdxs[1]] * beta 
                 + *normals[face->vertIdxs[2]] * gamma);
   normal.normalize();
	return normal;
}
