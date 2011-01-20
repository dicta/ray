#ifndef _WEDGE_H_
#define _WEDGE_H_

#include "Compound.h"

class Hash;

/**
 * Hash values:
 *    - innerRadius <i>double</i> - Radius from (0,0,0) to the inside edge of the wedges
 *    - outerRadius <i>double</i> - Radius from (0,0,0) to the outside edge of the wedges
 *    - bevelRadius <i>double</i> - Radius of the side spheres, cylinders, and tori
 *    - angle1 <i>double</i> - Starting angle of the wedge. angle 0 is on the positive Z axis.
 *    - angle2 <i>double</i> - Ending angle of the wedge.
 *    - minY <i>double</i> - Minimum Y value of the wedges
 *    - maxY <i>double</i> - Maximum Y value of the wedges
 *    - cover [ "true" | "false" ] - If set to false, a skeletal wedge is rendered
 *    - material {} - Material hash
 */
class Wedge : public Compound {

public:
   Wedge();
   Wedge(float ir, float outr, float br, float a1, float a2, float y1, float y2);
   virtual ~Wedge();
   
   virtual void setHash(Hash* hash);
   
private:
   void build();

   float innerR;
   float outerR;
   float bevelR;
   float angle1;
   float angle2;
   float minY;
   float maxY;
   bool cover;
};

#endif
