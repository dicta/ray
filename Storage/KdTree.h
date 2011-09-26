#ifndef KDTREE_H
#define KDTREE_H

#include "Storage.h"
#include "Geometry/BBox.h"

typedef unsigned int uint32_t;

struct KdNode {
   KdNode(list<GeometryObject*> _objs, const BBox& bounds, int _depth);
   KdNode(KdNode* l, KdNode* r, int _axis, double _split, const BBox& bounds, int _depth);

   bool isLeaf() const;

   KdNode* left;
   KdNode* right;
   list<GeometryObject*> objs;
   int axis;
   double split;
   const BBox& bbox;
   int depth;
};

struct BoundEdge {
   void set(float t, bool s) { tsplit = t; type = s ? START : END; }
   bool operator< (const BoundEdge& e) const;

   float tsplit;
   enum { START, END } type;
};


class SurfaceAreaHeuristic {
public:

  /* Precomputes some information that's constant across all possible splits. */
  SurfaceAreaHeuristic(const BBox &box) {
     double xSize = box.width(0);
     double ySize = box.width(1);
     double zSize = box.width(2);
     surfaceArea = 2.0 * (xSize*ySize + ySize*zSize + zSize*xSize);

     vec0 = Vector3D(ySize*zSize, xSize*zSize, xSize*ySize);
     vec0 *= (1.0 / surfaceArea);

     vec1 = Vector3D(ySize+zSize, xSize+zSize, xSize+ySize);
     vec1 *= (1.0 / surfaceArea);

  }

  inline float getSurfaceArea() {
    return surfaceArea;
  }

  /* Given the split point t along the bounding box into segments of width leftWidth & rightWidth,
   * compute the probability that we hit either child using the SAH.
   */
  inline std::pair<float, float> operator()(int axis, float leftWidth, float rightWidth) const {
     return std::pair<float, float>(vec0.get(axis) + vec1.get(axis) * leftWidth,
                                    vec0.get(axis) + vec1.get(axis) * rightWidth);
  }

private:      
   Vector3D vec0;
   Vector3D vec1;
   float    surfaceArea;
};

class KdTree : public Storage {

public:
   KdTree();
   virtual ~KdTree();

   virtual void setup();

   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;

private:
   KdNode* buildTree(int depth, list<GeometryObject*> objs, const BBox& bounds);
   bool checkNode(const Ray& ray, KdNode* node, double& tmin, ShadeRecord& sr) const;
   bool checkNodeShadow(const Ray& ray, KdNode* node, double& tmin) const;
   void findSplit(list<GeometryObject*>& objs, const BBox& bounds, int& axis, double& split);

   KdNode *root;
   BoundEdge* edges;


   /**
    * Tuning parameters for building the tree.
    */
   const static float costTraverse = 5.0f;
   const static float costQuery    = 30.0f;
   const static float emptySpace   = 0.2;

   size_t num_copies;
};

#endif // KDTREE_H
