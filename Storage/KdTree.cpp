#include "KdTree.h"
#include "Math/Maths.h"
#include <stack>

typedef vector<GeometryObject*>::iterator GeomIter;
typedef vector<int>::const_iterator IdxIter;

int MAX_DEPTH = 10;
const int MAX_OBJS = 10;

struct NodeS {
   KdNode* node;
   double min, max;

   NodeS(KdNode* n, double _min, double _max) {
      node = n;
      min = _min;
      max = _max;
   }
};

KdNode::KdNode(vector<int> _idxs) : left(NULL), right(NULL), idxs(_idxs) {
}

KdNode::KdNode(KdNode* l, KdNode* r, int _axis, double _split) :
   left(l),
   right(r),
   idxs(),
   axis(_axis),
   split(_split)
{
}

bool KdNode::isLeaf() const {
   return left == NULL && right == NULL;
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
   for(unsigned int i = 0; i < primitives.size(); i++) {
      idxs.push_back(i);
   }
   root = buildTree(0, idxs, bbox);
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

   return new KdNode(buildTree(depth+1, lidxs, left), buildTree(depth+1, ridxs, right), axis, split);
}

void KdTree::setHash(Hash* hash) {
   for(GeomIter it = primitives.begin(); it != primitives.end(); ++it) {
      (*it)->setHash(hash);
   }
}

bool KdTree::hit(const Ray& ray, double& tmin, ShadeRecord& sr) const {
   stack<NodeS> nodeStack;
   nodeStack.push(NodeS(root, 0, HUGE_VALUE));

   double tHit = HUGE_VALUE;

   while(!nodeStack.empty()) {
      KdNode* node = nodeStack.top().node;
      double min = nodeStack.top().min;
      double max = nodeStack.top().max;
      nodeStack.pop();

      while(!node->isLeaf()) {
         double originAxis = ray.origin.get(node->axis);
         double directionAxis = ray.direction.get(node->axis);
         double tsplit = (node->split - originAxis) / directionAxis;
         KdNode *first = node->left, *second = node->right;

         bool belowFirst = (originAxis < node->split) || (originAxis == node->split && directionAxis >= 0);
         if(!belowFirst) {
            first = node->right;
            second = node->left;
         }

         if(tsplit > max || tsplit < 0) {
            node = first;
         }
         else if(tsplit <= min) {
            node = second;
         }
         else {
            nodeStack.push(NodeS(second, tsplit, max));
            node = first;
            max = tsplit;
         }
      }
      if(checkNode(ray, node, tHit, sr) && (tHit < max)) {
         tmin = tHit;
         return true;
      }
   }
   return false;
/*
stack.push(root,sceneMin,sceneMax)
tHit=infinity
while (not stack.empty()):
   (node,tMin,tMax)=stack.pop()
   while (not node.isLeaf()):
      a = node.axis
      tSplit = ( node.value - ray.origin[a] ) / ray.direction[a]
      (first, sec) = order(ray.direction[a], node.left, node.right)
      if( tSplit >= tMax or tSplit < 0)
         node=first
      else if( tSplit <= tMin)
         node=second
      else
         stack.push( sec, tSplit, tMax)
         node=first
         tMax=tSplit
   for tri in node.triangles():
      tHit=min(tHit,tri.Intersect(ray))
      if tHit<tMax:
         return tHit //early exit
return tHitt
*/
}

bool KdTree::checkNode(const Ray& ray, KdNode* node, double& tmin, ShadeRecord& sr) const {
   bool hit = false;
   Vector3D normal;
   Point3D hitPoint;
   Point3D localHitPoint;
   shared_ptr<Material> mat;
   double t;

   for(IdxIter it = node->idxs.begin(); it != node->idxs.end(); it++) {
      GeometryObject* obj = primitives[*it];
      if(obj->hit(ray, t, sr) && (t < tmin)) {
         tmin = t;
         mat = obj->getMaterial();
         localHitPoint = sr.localHitPoint;
         normal = sr.normal;
         hitPoint = ray(tmin);
         hit = true;
      }
   }

   if(hit) {
      sr.localHitPoint = localHitPoint;
      sr.hitPoint = hitPoint;
      sr.normal = normal;
      sr.material = mat;
      sr.t = tmin;
   }
   return hit;
}

bool KdTree::shadowHit(const Ray& ray, double& tmin) const {
   ShadeRecord sr;
   return hit(ray, tmin, sr);
}

void KdTree::addObject(GeometryObject* obj) {
   primitives.push_back(obj);
   bbox.expand(obj->bbox);
}
