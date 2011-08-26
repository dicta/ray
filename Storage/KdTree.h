#ifndef KDTREE_H
#define KDTREE_H

#include "Geometry/GeometryObject.h"

typedef unsigned int uint32_t;

struct KdNode {
   KdNode(vector<int> _idxs);
   KdNode(KdNode* l, KdNode* r, int _axis, double _split);

   bool isLeaf() const;

   KdNode* left;
   KdNode* right;
   vector<int> idxs;
   int axis;
   double split;
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
   KdNode* buildTree(int depth, vector<int> idxs, const BBox& bbox);
   bool checkNode(const Ray& ray, KdNode* node, double& tmin, ShadeRecord& sr) const;

   vector<GeometryObject*> primitives;
   KdNode *root;
};

#endif // KDTREE_H
