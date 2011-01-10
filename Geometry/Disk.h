/*
 *  Disk.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/26/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _DISK_H_
#define _DISK_H_

class Sampler;

#include "LightObject.h"

/**
 * Created a disk object.
 *
 * Hash values:
 * <ul>
 * <li>center - Center of the disk. Array of doubles of the form [ x y z ]
 * <li>normal - Normal of the disk. Array of doubles of the form [ x y z ]
 * <li>radius - Radius of the disk. Double.
 * </ul>
 */
class Disk : public LightObject {
   
public:
   Disk();
   virtual ~Disk();
   
   virtual void setHash(Hash* hash);
   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const;
   virtual bool shadowHit(const Ray& ray, double& tmin) const;
   
   virtual Point3D sample() const;
   virtual Vector3D getNormal(const Point3D& point) const;
   virtual double pdf(const ShadeRecord& sr) const;
   
private:
   Point3D center;
   Vector3D normal;
   float radius;
   float radiusSquared;
   Sampler* sampler;
   double inverseArea;
   Vector3D a, b;
};

#endif
