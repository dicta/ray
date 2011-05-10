#ifndef _GEOSPHERE_H_
#define _GEOSPHERE_H_

#include "Mesh.h"
#include <map>
#include <vector>

using namespace std;

class Edge {
   
public:
   Edge(int p1, int p2);
   Edge(const Edge& e);
   bool operator==(const Edge& e) const;
   bool operator<(const Edge& e) const;
   
   int pidx1, pidx2;
};


class GeoSphere : public Mesh {
   
public:
   GeoSphere();
   virtual ~GeoSphere();
   virtual void setHash(Hash* hash);

private:
   void subdivide(int pidx1, int idxp2, int pidx3);
   vector<int>* getEdgePoints(const Edge& e);
   void createPoint(double x, double y, double z);
   
   int divs;
   map<Edge, vector<int>* > edgeMap;
};

#endif
