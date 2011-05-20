/*
 *  GeometryManager.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _GEOMETRY_MANAGER_H_
#define _GEOMETRY_MANAGER_H_

#include "Storage/Grid.h"
#include <vector>
#include <memory>
#include <string>

using namespace std;

class GeometryObject;
class Hash;

typedef vector<GeometryObject*>::const_iterator GeometryIter;

class GeometryManager {
   
public:
   static GeometryManager& instance();
   ~GeometryManager();
   
   void loadObjects(string fname);

   GeometryIter begin() const { return objects.begin(); }
   GeometryIter end() const { return objects.end(); }
   
   GeometryObject* createObject(string type, Hash* hash, bool addToList = true);
   Grid getGrid() const { return grid; }
   
private:
   GeometryManager();
   
   static auto_ptr<GeometryManager> s_instance;
   vector<GeometryObject*> objects;
   Grid grid;
};

#endif
