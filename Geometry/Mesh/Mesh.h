#ifndef _MESH_H_
#define _MESH_H_

#include "Math/Point3D.h"
#include "Math/Point2D.h"
#include "Math/Vector3D.h"
#include "Geometry/GeometryObject.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

class Face {

public:
   Face(int idx1, int idx2, int idx3);

   Vector3D normal;
   BBox bbox;
   int smoothGroup;
   Material* material;
   int vertIdxs[3];
   Vector3D dpdu;
   Vector3D dpdv;
};

struct Voxel {
   ~Voxel() { faces.clear(); }
   vector<Face*> faces;
   void add(Face* f) { faces.push_back(f); }
};

class SmoothingGroup {
public:
   ~SmoothingGroup();

   void addFace(Face* f);
   void normalize();
   Vector3D interpolateNormal(Face* face, const double beta, const double gamma);
   Vector3D interpolateDPDU(Face* face, const double beta, const double gamma);
   Vector3D interpolateDPDV(Face* face, const double beta, const double gamma);

   map<int, Vector3D*> normals;
   map<int, Vector3D*> dpdu;
   map<int, Vector3D*> dpdv;
};

typedef vector<Face*>::const_iterator FaceIter;

class Mesh : public GeometryObject {

public:
   Mesh();
   virtual ~Mesh();

   void pointsReserve(int size) { points.reserve(size); }
   int addPoint(Point3D* p);

   void facesReserve(int size) { faces.reserve(size); }
   void addFace(Face* f);

   void textureCoordsReserve(int size) { textureCoords.reserve(size); }
   void addTextureCoord(float u, float v) { textureCoords.push_back(new Point2D(u, v)); }

   FaceIter facesBegin() const { return faces.begin(); }
   FaceIter facesEnd() const { return faces.end(); }
   void setFaceMaterial(int idx, Material* material);

   void calculateNormals();

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;

   void setupCells();
   string name;
   map<unsigned int, SmoothingGroup*> smoothingGroups;

protected:
   bool hitFace(Face* face, const Ray& ray, double& tmin, ShadeRecord& sr) const;
   Vector3D interpolateNormal(Face* face, const double beta, const double gamma) const;
   int getPointCount() const { return points.size(); }
   Point3D* getPointAt(int idx) const { return points[idx]; }

   int numCells;
   vector<Point3D*> points;
   vector<Vector3D*> normals;
   vector<Face*> faces;
   vector<Point2D*> textureCoords;
   Voxel** voxels;

   int nx, ny, nz;

private:
   double calculateNext(double rd, double min, double i, double dt, int n, int& step, int& stop) const;
   bool checkCell(const Ray& ray, Voxel* cell, double& tmin, ShadeRecord& sr) const;
   void computePartialDerivitives(Face* face, const Vector3D& e1, const Vector3D& e2) const;
   void getUVs(double uv[3][2], Face* face) const;
};

#endif
