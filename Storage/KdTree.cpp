#include "KdTree.h"
#include "Math/Maths.h"
#include <stack>
#include <limits>
#include "Utility/PerfCounter.h"
#include "Materials/Matte.h"
#include "Geometry/Box.h"

typedef list<GeometryObject*>::iterator GeomIter;

#define DEBUG_KDTREE 0

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

KdNode::KdNode(list<GeometryObject*> _objs, const BBox& bounds, int _depth) :
  left(NULL), right(NULL), objs(_objs), bbox(bounds), depth(_depth) {
}

KdNode::KdNode(KdNode* l, KdNode* r, int _axis, double _split, const BBox& bounds, int _depth) :
   left(l),
   right(r),
   objs(),
   axis(_axis),
   split(_split),
   bbox(bounds),
   depth(_depth)
{
}

bool KdNode::isLeaf() const {
   return left == NULL && right == NULL;
}

KdTree::KdTree() :
   Storage(),
   root(NULL),
   edges(NULL),
   num_copies(0)
{
}

KdTree::~KdTree() {
}

void KdTree::setup() {
   MAX_DEPTH = int(8 + 1.3f * Log2Int(float(objs.size())));
   edges = new BoundEdge[objs.size() * 2];
   
   struct timespec start;
   struct timespec finish;
   
   printf("KdTree build started (%lu objects).\n", objs.size());
   clock_gettime(CLOCK_MONOTONIC, &start);
   root = buildTree(0, objs, bbox);
   clock_gettime(CLOCK_MONOTONIC, &finish);
   double duration = (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) * 1e-9;
   printf("KdTree build complete (%.4f sec). %lu copies of objects into multiple nodes.\n", duration, num_copies);
}

void KdTree::findSplit(list<GeometryObject*>& objs, const BBox& bounds, int& axis, double& split) {
  //   double tsplit = numeric_limits<double>::infinity();
   int taxis = bounds.maxExtentAxis();
   int tries = 0;
   bool found = false;
   axis = -1;

   SurfaceAreaHeuristic heuristic(bounds);

   while(!found && tries < 3) {
      int idx = 0;
      for(GeomIter it = objs.begin(); it != objs.end(); ++it) {
         edges[2 * idx].set((*it)->bbox.getMin(taxis), true);
         edges[2 * idx + 1].set((*it)->bbox.getMax(taxis), false);
         idx++;
      }
      sort(&edges[0], &edges[2 * objs.size()]);

      int nBelow = 0, nAbove = objs.size();
      float bestCost = numeric_limits<double>::max();
      
      float minValue = bounds.getMin(taxis);
      float maxValue = bounds.getMax(taxis);
      for(unsigned i = 0; i < 2 * objs.size(); i++) {
         if(edges[i].type == BoundEdge::END) nAbove--;
         float edget = edges[i].tsplit;
         if(edget > minValue && edget < maxValue) {
            float lowerWidth = edget - minValue;
            float upperWidth = maxValue - edget;
            const std::pair<float, float>& p = heuristic(taxis, lowerWidth, upperWidth);
            float cost = costTraverse + costQuery * (p.first * nBelow + p.second * nAbove);
            if (nBelow == 0 || nAbove == 0) 
              cost *= emptySpace;

            if(cost < bestCost) {
               bestCost = cost;
               split = edget;
               axis = taxis;
               found = bestCost < (costQuery * objs.size());
            }
         }
         if(edges[i].type == BoundEdge::START) nBelow++;
      }
      
      if(!found) {
         tries++;
         taxis = (taxis+1) % 3;
      }
   }
   if(!found) {
     axis = -1;
   }
}

KdNode* KdTree::buildTree(int depth, list<GeometryObject*> objs, const BBox& bounds) {
  //printf("KdTree build: depth = %3d\t num_objects = %4lu\n", depth, objs.size());
   if(depth >= MAX_DEPTH || objs.size() < MAX_OBJS) {
      // stop recursion
      return new KdNode(objs, bounds, depth);
   }

   int axis;
   double split;
   findSplit(objs, bounds, axis, split);

   if(axis == -1) {
      return new KdNode(objs, bounds, depth);
   }

   BBox left = bounds;
   left.setMax(axis, split);
   BBox right = bounds;
   right.setMin(axis, split);

   list<GeometryObject*> lidxs, ridxs;
   for(GeomIter it = objs.begin(); it != objs.end(); ++it) {
      bool inLeft  = left.intersects((*it)->bbox);
      bool inRight = right.intersects((*it)->bbox);
      if(inLeft) {
         lidxs.push_back(*it);
      }
      if(inRight) {
         ridxs.push_back(*it);
      }

      if (inLeft && inRight) {
        num_copies++;
      }
   }

   return new KdNode(buildTree(depth+1, lidxs, left), buildTree(depth+1, ridxs, right), axis, split, bounds, depth);
}

void KdTree::setHash(Hash* hash) {
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
   double tcheck = HUGE_VALUE;

   performance_counter.increment_primary_nodes_traversed();

#if DEBUG_KDTREE
   Point3D p0 = Point3D(node->bbox.x0, node->bbox.y0, node->bbox.z0);
   Point3D p1 = Point3D(node->bbox.x1, node->bbox.y1, node->bbox.z1);
   float red   = clamp(node->depth / 12.0, 0.0, 0.66);
   float green = clamp((node->depth - 12) / 12.0, 0.0, 1.0);
   float blue  = clamp((node->depth - 24) / 12.0, 0.0, 1.0);
   float alpha = clamp(node->depth / 15.0, 0.15, 1.0);

   Box b = Box(p0,p1);
   if (b.hit(ray,tmin,sr) && (tmin < tcheck)) {
     tcheck = tmin;
     Matte* matte = new Matte();
     matte->setAmbientColor(new Color(red,green,blue,alpha));
     matte->setDiffuseColor(new Color(red,green,blue,alpha));
     mat = shared_ptr<Material>(matte);
     localHitPoint = sr.localHitPoint;
     normal = sr.normal;
     hitPoint = ray(tmin);
     hit = true;
   }
#endif

   for(GeomIter it = node->objs.begin(); it != node->objs.end(); it++) {
      if((*it)->hit(ray, tmin, sr) && (tmin < tcheck)) {
         tcheck = tmin;
         mat = (*it)->getMaterial();
         localHitPoint = sr.localHitPoint;
         normal = sr.normal;
         hitPoint = ray(tmin);
         hit = true;
         performance_counter.increment_primary_hits();
      } else {
         performance_counter.increment_primary_misses();
      }
   }

   if(hit) {
      sr.localHitPoint = localHitPoint;
      sr.hitPoint = hitPoint;
      sr.normal = normal;
      sr.material = mat;
   }
   return hit;
}

bool KdTree::shadowHit(const Ray& ray, double& tmin) const {
   performance_counter.increment_num_shadow_rays();

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
      if(checkNodeShadow(ray, node, tHit) && (tHit < max)) {
         tmin = tHit;
         return true;
      }
   }
   return false;
}

bool KdTree::checkNodeShadow(const Ray& ray, KdNode* node, double& tmin) const {
   bool hit = false;
   double tcheck = HUGE_VALUE;
   
   performance_counter.increment_shadow_nodes_traversed();
   for(GeomIter it = node->objs.begin(); it != node->objs.end(); it++) {
      if(!(*it)->ignoreShadow && (*it)->shadowHit(ray, tmin) && (tmin < tcheck)) {
         tcheck = tmin;
         hit = true;
         performance_counter.increment_shadow_hits();
      } else {
         performance_counter.increment_shadow_misses();
      }
   }

   return hit;
}
