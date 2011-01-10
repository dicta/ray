#ifndef _MESH_H_
#define _MESH_H_

#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "Geometry/GeometryObject.h"
#include "Geometry/BBox.h"
#include <vector>

using namespace std;

class Face {
   
public:
   Face(int idx1, int idx2, int idx3) {
      vertIdxs[0] = idx1;
      vertIdxs[1] = idx2;
      vertIdxs[2] = idx3;
   }

   int vertIdxs[3];
};

typedef vector<Face*>::const_iterator FaceIter;

class Mesh : public GeometryObject {
   
public:
   Mesh();
   virtual ~Mesh();

   int addPoint(Point3D* p);
   void addFace(Face* f);
   void calculateNormals();
   
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
private:
   bool hitFace(Face* face, const Ray& ray, double& tmin, ShadeRecord& sr) const;
   Vector3D interpolateNormal(Face* face, const double beta, const double gamma) const;

   vector<Point3D*> points;
   vector<Vector3D*> normals;
   vector<Face*> faces;
   BBox bbox;
};

#endif
