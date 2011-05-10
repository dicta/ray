#include "GeoSphere.h"
#include <math.h>
#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "Parser/Hash.h"

const double GOLDEN = (1.0 + sqrt(5.0)) / 2.0;

const double DEG_TO_RAD = M_PI / 180.0;

typedef map<Edge, vector<int>* >::const_iterator EdgeMapIter;

GeoSphere::GeoSphere() {
}

void GeoSphere::setHash(Hash* hash) {
   divs = hash->getValue("divs")->getInteger();
   // ALl points are rotated so that (0, GOLDEN, 1) is the north pole and is rotated to (0, 1, 0)
   // 90 degrees = height of a triangle + half the length of a triangle side
   // side = 2.0
   // traingle height = side * sin(60)
   double ninety_degrees = 2.0 * sin(60 * DEG_TO_RAD) + 1.0;
   double rotate = 1.0 / ninety_degrees * -90.0 * DEG_TO_RAD;
   
   // rotate_y = y * cos(rotate) - z * sin(rotate)
   // rotate_z = y * sin(rotate) + z * cos(rotate)
   // cache cos(rotate) and sin(rotate)
   double c_rotate = cos(rotate);
   double s_rotate = sin(rotate);
   
   // Define the north pole
   createPoint(0, GOLDEN * c_rotate - s_rotate, GOLDEN * s_rotate + c_rotate); // (0, GOLDEN, 1)

   // Row 2
   createPoint(-1, -GOLDEN * s_rotate, GOLDEN * c_rotate); // (-1, 0, GOLDEN)
   createPoint(1, -GOLDEN * s_rotate, GOLDEN * c_rotate); // (1, 0, GOLDEN)
   createPoint(GOLDEN, c_rotate, s_rotate); // (GOLDEN, 1, 0)
   createPoint(0, GOLDEN * c_rotate + s_rotate, GOLDEN * s_rotate - c_rotate); // (0, GOLDEN, -1)
   createPoint(-GOLDEN, c_rotate, s_rotate); // (-GOLDEN, 1, 0)
   
   // Row 3
   createPoint(-GOLDEN, -c_rotate, -s_rotate); // (-GOLDEN, -1, 0)
   createPoint(0, -GOLDEN * c_rotate - s_rotate, -GOLDEN * s_rotate + c_rotate); // (0, -GOLDEN, 1)
   createPoint(GOLDEN, -c_rotate, -s_rotate); // (GOLDEN, -1, 0)
   createPoint(1, GOLDEN * s_rotate, -GOLDEN * c_rotate); // (1, 0, -GOLDEN)
   createPoint(-1, GOLDEN * s_rotate, -GOLDEN * c_rotate); // (-1. 0. -GOLDEN)
   
   // Define the south pole
   createPoint(0, -GOLDEN * c_rotate + s_rotate, -GOLDEN * s_rotate - c_rotate); // (0, -GOLDEN, -1)
   
   // North pole faces   
   subdivide(0, 1, 2);
   subdivide(0, 2, 3);
   subdivide(0, 3, 4);
   subdivide(0, 4, 5);
   subdivide(0, 5, 1);

   // Mid row faces
   subdivide(1, 6, 7);
   subdivide(7, 2, 1);
   subdivide(2, 7, 8);
   subdivide(8, 3, 2);
   subdivide(3, 8, 9);
   subdivide(9, 4, 3);
   subdivide(4, 9, 10);
   subdivide(10, 5, 4);
   subdivide(5, 10, 6);
   subdivide(6, 1, 5);

   // South pole faces
   subdivide(6, 11, 7);
   subdivide(7, 11, 8);
   subdivide(8, 11, 9);
   subdivide(9, 11, 10);
   subdivide(10, 11, 6);

   Mesh::setHash(hash);
//   printf("face count = %d\n", mesh->getFaceCount());
}

GeoSphere::~GeoSphere() {
}

void GeoSphere::subdivide(int pidx1, int pidx2, int pidx3) {
   if(divs == 1) {
      addFace(new Face(pidx1, pidx2, pidx3));
      return;
   }

   vector<int>* side12 = getEdgePoints(Edge(pidx1, pidx2));
   vector<int>* side13 = getEdgePoints(Edge(pidx1, pidx3));
   vector<int>* side23 = getEdgePoints(Edge(pidx2, pidx3));
   int** span = new int*[divs+1];
   for(int i = 0; i < divs+1; i++) {
      span[i] = new int[divs];
   }
   
   int pidx = getPointCount();

   for(int i = 1; i < (divs - 1); i++) {
      Point3D* p1 = getPointAt(side12->at(divs - i));
      Point3D* p2 = getPointAt(side13->at(divs - i));

      for(int j = 1; j < (divs - i); j++) {
         double d = (double)j / (double)(divs - i);
         double x = p1->x + d * (p2->x - p1->x);
         double y = p1->y + d * (p2->y - p1->y);
         double z = p1->z + d * (p2->z - p1->z);

         createPoint(x, y, z);
         span[i][j] = pidx++;
      }
   }
   
   // Top face
   Face* f = new Face(pidx1, side12->at(1), side13->at(1));
   addFace(f);

   if(divs == 2) {
      f = new Face(side12->at(1), side12->at(2), side23->at(1));
      addFace(f);
   
      f = new Face(side12->at(1), side23->at(1), side13->at(1));
      addFace(f);
   
      f = new Face(side13->at(1), side23->at(1), side13->at(2));
      addFace(f);
      return;
   }
   
   // Row two faces
   f = new Face(side12->at(1), side12->at(2), span[divs-2][1]);
   addFace(f);
   
   f = new Face(side12->at(1), span[divs-2][1], side13->at(1));
   addFace(f);
   
   f = new Face(side13->at(1), span[divs-2][1], side13->at(2));
   addFace(f);

   // Center faces
   for(int i = 2; i < divs-1; i++) {      
      f = new Face(side12->at(i), side12->at(i+1), span[divs-i-1][1]);
      addFace(f);
      
      f = new Face(side12->at(i), span[divs-i-1][1], span[divs-i][1]);
      addFace(f);
      
      for(int j = 1; j < i; j++) {
         f = new Face(span[divs-i][j], span[divs-i-1][j], span[divs-i-1][j+1]);
         addFace(f);

         if(j < i-1) {
            f = new Face(span[divs-i][j], span[divs-i-1][j+1], span[divs-i][j+1]);
            addFace(f);
         }
      }

      f = new Face(side13->at(i), span[divs-i][i-1], span[divs-i-1][i]);
      addFace(f);
      
      f = new Face(side13->at(i), span[divs-i-1][i], side13->at(i+1));
      addFace(f);
   }
   
   // Bottom row faces
   f = new Face(side12->at(divs-1), pidx2, side23->at(1));
   addFace(f);

   f = new Face(side12->at(divs-1), side23->at(1), span[1][1]);
   addFace(f);
   
   for(int j = 2; j < divs; j++) {
      f = new Face(span[1][j-1], side23->at(j-1), side23->at(j));
      addFace(f);
      
      if(j < divs-1) {
         f = new Face(span[1][j-1], side23->at(j), span[1][j]);         
         addFace(f);
      }
   }
   
   f = new Face(span[1][divs-2], side23->at(divs-1), side13->at(divs-1));
   addFace(f);
   
   f = new Face(side13->at(divs-1), side23->at(divs-1), pidx3);
   addFace(f);
}

vector<int>* GeoSphere::getEdgePoints(const Edge& e) {
   int pidx = getPointCount();

   Point3D* p1 = getPointAt(e.pidx1);
   Point3D* p2 = getPointAt(e.pidx2);

   EdgeMapIter iter = edgeMap.find(e);
   vector<int>* points;

   if(iter == edgeMap.end()) {
      points = new vector<int>();
      points->push_back(e.pidx1);

      for(int i = 1; i < divs; i++) {
         double d = (double)i / (double)divs;
         double x = p1->x + d * (p2->x - p1->x);
         double y = p1->y + d * (p2->y - p1->y);
         double z = p1->z + d * (p2->z - p1->z);
         
         createPoint(x, y, z);
         points->push_back(pidx++);
      }
      
      points->push_back(e.pidx2);
      edgeMap[e] = points;
   }
   else {
      points = edgeMap[e];
   }
   return points;
}

void GeoSphere::createPoint(double x, double y, double z) {
   Vector3D v(x, y, z);
   v.normalize();
   addPoint(new Point3D(v.x, v.y, v.z));
}

Edge::Edge(int p1, int p2) : pidx1(p1), pidx2(p2) {
}

Edge::Edge(const Edge& e) {
   pidx1 = e.pidx1;
   pidx2 = e.pidx2;
}

bool Edge::operator==(const Edge& e) const {
   if(e.pidx1 != pidx1) {
      return false;
   }
   if(e.pidx2 != pidx2) {
      return false;
   }
   return true;
}

bool Edge::operator<(const Edge& e) const {
   if(pidx1 < e.pidx1) {
      return true;
   }
   else if(pidx1 > e.pidx1) {
      return false;
   }
   
   if(pidx2 < e.pidx2) {
      return true;
   }
   return false;
}
