#include "WedgeRing.h"
#include "Parser/Hash.h"

WedgeRing::WedgeRing() : Compound() {
}

WedgeRing::~WedgeRing() {
}

void WedgeRing::setHash(Hash* hash) {
   int count = hash->getInteger("count");
   float innerR = hash->getDouble("innerRadius");
   float outerR = hash->getDouble("outerRadius");
   float bevelR = hash->getDouble("bevelRadius");
   float minY = hash->getDouble("minY");
   float maxY = hash->getDouble("maxY");
   
   float angleSize = 360.0 / count;
   float angle1 = 0.0;
   float angle2 = angleSize;
   
   for(int i = 0;i < count; i++) {
      Wedge* w = new Wedge(innerR, outerR, bevelR, angle1, angle2, minY, maxY);
      bbox.expand(w->getBBox());

      objects.push_back(w);
      angle1 += angleSize;
      angle2 += angleSize;
   }
   
   setupMaterial(hash->getValue("material")->getHash());
}
