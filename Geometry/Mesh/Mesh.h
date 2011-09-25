#ifndef _MESH_H_
#define _MESH_H_

#include "Math/Point3D.h"
#include "Math/Point2D.h"
#include "Math/Vector3D.h"
#include "Geometry/GeometryObject.h"
#include "Geometry/Compound/Compound.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

class Mesh;

class Face : public GeometryObject {

public:
   Face(Mesh& mesh, int idx1, int idx2, int idx3);

   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   virtual void setHash(Hash* hash) {}

   int vertIdxs[3];
   Vector3D p1p2;
   Vector3D p1p3;
   Vector3D normal;
   Vector3D dpdu;
   Vector3D dpdv;
   int smoothGroup;
   Mesh& parent;
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
   Vector3D interpolateNormal(const Face* face, const double beta, const double gamma);
   Vector3D interpolateDPDU(const Face* face, const double beta, const double gamma);
   Vector3D interpolateDPDV(const Face* face, const double beta, const double gamma);

   map<int, Vector3D*> normals;
   map<int, Vector3D*> dpdu;
   map<int, Vector3D*> dpdv;
};

typedef vector<Face*>::const_iterator FaceIter;

class Mesh : public Compound {

public:
   Mesh();
   virtual ~Mesh();

   virtual bool isCompound() { return false; } 
   virtual vector<GeometryObject*> getObjects() const { return vector<GeometryObject*>(faces.begin(), faces.end()); }

   void pointsReserve(int size) { points.reserve(size); }
   int addPoint(Point3D* p);

   void facesReserve(int size) { faces.reserve(size); }
   void addFace(int v1, int v2, int v3);

   void textureCoordsReserve(int size) { textureCoords.reserve(size); }
   void addTextureCoord(float u, float v) { textureCoords.push_back(Point2D(u, v)); }

   FaceIter facesBegin() const { return faces.begin(); }
   FaceIter facesEnd() const { return faces.end(); }
   void setFaceMaterial(int idx, shared_ptr<Material> material);
   Point3D* getPointAt(int idx) const { return points.at(idx); }

   void calculateNormals();

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;

   void setupCells();
   string name;
   map<unsigned int, SmoothingGroup*> smoothingGroups;
   vector<Point2D> textureCoords;

protected:
   Vector3D interpolateNormal(Face* face, const double beta, const double gamma) const;
   int getPointCount() const { return points.size(); }

   int numCells;
   vector<Point3D*> points;
   vector<Vector3D*> normals;
   vector<Face*> faces;
   Voxel** voxels;

   int nx, ny, nz;

private:
   double calculateNext(double rd, double min, double i, double dt, int n, int& step, int& stop) const;
   bool checkCell(const Ray& ray, Voxel* cell, double& tmin, ShadeRecord& sr) const;
   void computePartialDerivitives(Face* face) const;
   void getUVs(double uv[3][2], Face* face) const;
};

#endif
