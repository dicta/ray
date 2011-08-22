#include "KdTree.h"
#include "Math/Maths.h"

typedef vector<GeometryObject*>::iterator GeomIter;

int MAX_DEPTH = 10;
const int MAX_OBJS = 10;

KdNode::KdNode(vector<int> _idxs) : left(NULL), right(NULL), idxs(_idxs) {
}

KdNode::KdNode(KdNode* l, KdNode* r) : left(l), right(r), idxs() {
}

KdTree::KdTree() :
   GeometryObject(),
   primitives(),
   root(NULL)
{
}

KdTree::~KdTree() {
}

void KdTree::buildTree() {
   MAX_DEPTH = int(8 + 1.3f * Log2Int(float(primitives.size())));
   vector<int> idxs;
   for(int i = 0; i < primitives.size(); i++) {
      idxs.push_back(i);
   }
   buildTree(0, idxs, bbox);
}

KdNode* KdTree::buildTree(int depth, vector<int> idxs, const BBox& bbox) {
   if(depth >= MAX_DEPTH || idxs.size() < MAX_OBJS) {
      // stop recursion
      return new KdNode(idxs);
   }
   int axis = depth % 3;
   double split = (bbox.getMax(axis) - bbox.getMin(axis)) * 0.5;

   BBox left = bbox;
   left.setMax(axis, split);
   BBox right = bbox;
   right.setMin(axis, split);

   vector<int> lidxs, ridxs;
   for(vector<int>::iterator it = idxs.begin(); it != idxs.end(); ++it) {
      if(left.intersects(primitives[*it]->bbox)) {
         lidxs.push_back(*it);
      }
      if(right.intersects(primitives[*it]->bbox)) {
         ridxs.push_back(*it);
      }
   }

   return new KdNode(buildTree(depth+1, lidxs, left), buildTree(depth+1, ridxs, right));
}

void KdTree::setHash(Hash* hash) {
   for(GeomIter it = primitives.begin(); it != primitives.end(); ++it) {
      (*it)->setHash(hash);
   }
}

bool KdTree::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   return false;
}

bool KdTree::shadowHit(const Ray& ray, double& tmin) const {
   ShadeRecord sr;
   return hit(ray, tmin, sr);
}

void KdTree::addObject(GeometryObject* obj) {
   primitives.push_back(obj);
   bbox.expand(obj->bbox);
}
