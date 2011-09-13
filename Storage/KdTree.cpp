#include "KdTree.h"
#include "Math/Maths.h"
#include <stack>
#include <limits>

typedef vector<GeometryObject*>::iterator GeomIter;

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

bool BoundEdge::operator< (const BoundEdge& e) const {
   if(tsplit == e.tsplit) {
      return type < e.type;
   }
   return tsplit < e.tsplit;
}

KdNode::KdNode(vector<GeometryObject*> _objs) : left(NULL), right(NULL), objs(_objs) {
}

KdNode::KdNode(KdNode* l, KdNode* r, int _axis, double _split) :
   left(l),
   right(r),
   objs(),
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
   edges = new BoundEdge[primitives.size() * 2];
   root = buildTree(0, primitives, bbox);
   printf("done build\n");
}

void KdTree::findSplit(vector<GeometryObject*>& objs, const BBox& bounds, int& axis, double& split) {
   double invTotalSA = 1.0 / bounds.surfaceArea();
   double tsplit = numeric_limits<double>::infinity();
   
   int taxis = bounds.maxExtentAxis();
   int tries = 0;
   bool found = false;
   
   axis = -1;
   
   while(!found && tries < 3) {
      int idx = 0;
      for(GeomIter it = objs.begin(); it != objs.end(); ++it) {
         edges[2 * idx].set((*it)->bbox.getMin(taxis), true);
         edges[2 * idx + 1].set((*it)->bbox.getMax(taxis), false);
         idx++;
      }
      sort(&edges[0], &edges[2 * objs.size()]);
   
      int nBelow = 0, nAbove = primitives.size();
      double bestCost = numeric_limits<double>::max();
   
      for(int i = 0; i < 2 * primitives.size(); i++) {
         if(edges[i].type == BoundEdge::END) nAbove--;
         double edget = edges[i].tsplit;
         if(edget > bounds.getMin(taxis) && edget < bounds.getMax(taxis)) {
            int other0 = (taxis + 1) % 3;
            int other1 = (taxis + 2) % 3;
         
            double belowSA = 2 * (bounds.width(other0) * bounds.width(other1)) +
                                 (edget - bounds.getMin(taxis)) *
                                 (bounds.width(other0) + bounds.width(other1));
         
            double aboveSA = 2 * (bounds.width(other0) * bounds.width(other1)) +
                                 (bounds.getMax(taxis) - edget) *
                                 (bounds.width(other0) + bounds.width(other1));
         
            double pBelow = belowSA * invTotalSA;
            double pAbove = aboveSA * invTotalSA;
            double eb = (nBelow == 0 || nAbove == 0) ? 0.2 : 1.0;

            double cost = 1.0 + 80.0 * eb * (pBelow * nBelow + pAbove * nAbove);
         
            if(cost < bestCost) {
               bestCost = cost;
               tsplit = edget;
               found = bestCost < (80.0 * primitives.size());
            }
         }
         if(edges[i].type == BoundEdge::START) nBelow++;
      }
      
      if(!found) {
         tries++;
         taxis = (taxis + 1) % 3;
      }
   }
   if(found) {
      axis = taxis;
      split = tsplit;
   }
}

KdNode* KdTree::buildTree(int depth, vector<GeometryObject*> objs, const BBox& bounds) {
   if(depth >= MAX_DEPTH || objs.size() < MAX_OBJS) {
      // stop recursion
      return new KdNode(objs);
   }
   
//   int axis = depth % 3;
//   double split = (bounds.getMax(axis) - bounds.getMin(axis)) * 0.5;
   int axis;
   double split;
   findSplit(objs, bounds, axis, split);
   
   if(axis == -1) {
      return new KdNode(objs);
   }

   BBox left = bounds;
   left.setMax(axis, split);
   BBox right = bounds;
   right.setMin(axis, split);

   vector<GeometryObject*> lidxs, ridxs;
   for(GeomIter it = objs.begin(); it != objs.end(); ++it) {
      if(left.intersects((*it)->bbox)) {
         lidxs.push_back(*it);
      }
      if(right.intersects((*it)->bbox)) {
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

   for(GeomIter it = node->objs.begin(); it != node->objs.end(); it++) {
      if((*it)->hit(ray, t, sr) && (t < tmin)) {
         tmin = t;
         mat = (*it)->getMaterial();
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
