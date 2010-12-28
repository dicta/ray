/*
 *  Sphere.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "GeometryObject.h"
#include "Math/Point3D.h"

/**
 * Creates a sphere or a part sphere.
 *
 * Hash values:
 * <ul>
 * <li>center - Array of 3 doubles of the form [ x y z ]
 * <li>radius - Single double value
 * <li>thetaRange (optional) - Latitude range tp include. Array of 2 daubles from 0 to 180. 0 is at the top of the sphere.
 * <li>phiRange (optional) - Longitude range to include. Array of 2 doubles from 0 to 360. 0 is in the positive Z axis.
 * </ul>
 */
class Sphere : public GeometryObject {
   
public:
   Sphere();

   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;   
   virtual void setHash(Hash* hash);
   
private:
   bool partCheck(const Ray& ray, double t) const;

   Point3D center;
   double radius;
   bool thetaRange, phiRange;
   double cosThetaMin, cosThetaMax;
   double phiMin, phiMax;
};

#endif
