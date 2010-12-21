/*
 *  GeometryObject.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _GEOMETRY_OBJECT_
#define _GEOMETRY_OBJECT_

#include "Math/Ray.h"
#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"
#include "Math/Point3D.h"

class Hash;
class Material;
class Hash;

class GeometryObject {
   
public:
   GeometryObject();
   virtual ~GeometryObject();

   virtual bool hit(const Ray& ray, double& tmin, ShadeRecord& sr) const = 0;
   virtual bool shadowHit(const Ray& ray, double& tmin) const = 0;

   void setupMaterial(Hash* hash);
   Material* getMaterial() const { return material; }
   
   virtual void setHash(Hash* hash) = 0;
   
protected:
   static const double epsilon;

private:
   Material* material;
};

#endif
