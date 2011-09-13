#ifndef KDTREE_H
#define KDTREE_H

#include "Geometry/GeometryObject.h"

typedef unsigned int uint32_t;

struct KdNode {
   KdNode(vector<GeometryObject*> _objs);
   KdNode(KdNode* l, KdNode* r, int _axis, double _split);

   bool isLeaf() const;

   KdNode* left;
   KdNode* right;
   vector<GeometryObject*> objs;
   int axis;
   double split;
};

struct BoundEdge {
   void set(float t, bool s) { tsplit = t; type = s ? START : END; }
   bool operator< (const BoundEdge& e) const;

   float tsplit;
   enum { START, END } type;
};

class KdTree : public GeometryObject {

public:
   KdTree();
   virtual ~KdTree();

   void addObject(GeometryObject* obj);
   void buildTree();

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;

private:
   KdNode* buildTree(int depth, vector<GeometryObject*> objs, const BBox& bounds);
   bool checkNode(const Ray& ray, KdNode* node, double& tmin, ShadeRecord& sr) const;
   void findSplit(vector<GeometryObject*>& objs, const BBox& bounds, int& axis, double& split);

   vector<GeometryObject*> primitives;
   KdNode *root;
   BoundEdge* edges;
};

#endif // KDTREE_H
