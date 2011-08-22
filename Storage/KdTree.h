#ifndef KDTREE_H
#define KDTREE_H

#include "Geometry/GeometryObject.h"

typedef unsigned int uint32_t;

struct KdNode {
   KdNode(vector<int> _idxs);
   KdNode(KdNode* l, KdNode* r);

   KdNode* left;
   KdNode* right;
   vector<int> idxs;
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

   vector<GeometryObject*> primitives;
   KdNode *root;
};

#endif // KDTREE_H
