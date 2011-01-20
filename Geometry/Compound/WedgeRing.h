#ifndef _WEDGE_RING_H_
#define _WEDGE_RING_H_

#include "Wedge.h"

/**
 * Create a ring of wedges. The first wedge starts with the left edge on the positive Z axis and
 * grows towards the positive X axis.
 *
 * Hash values:
 *    - count <i>integer</i> - Number of wedges to include in the ring
 *    - innerRadius <i>double</i> - Radius from (0,0,0) to the inside edge of the wedges
 *    - outerRadius <i>double</i> - Radius from (0,0,0) to the outside edge of the wedges
 *    - bevelRadius <i>double</i> - Radius of the side spheres, cylinders, and tori
 *    - minY <i>double</i> - Minimum Y value of the wedges
 *    - maxY <i>double</i> - Maximum Y value of the wedges
 *    - material {} - Material hash
 */
class WedgeRing : public Compound {

public:
   WedgeRing();
   virtual ~WedgeRing();
   
   virtual void setHash(Hash* hash);
};

#endif
