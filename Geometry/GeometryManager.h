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

#include <vector>
#include <memory>

using namespace std;

class GeometryObject;
class Hash;

typedef vector<GeometryObject*>::const_iterator GeometryIter;

class GeometryManager {
   
public:
   static GeometryManager& instance();
   ~GeometryManager();
   
   GeometryIter begin() const { return objects.begin(); }
   GeometryIter end() const { return objects.end(); }
   
   GeometryObject* createObject(string type, Hash* hash);
   
private:
   GeometryManager();
   void loadScene();
   
   static auto_ptr<GeometryManager> s_instance;
   vector<GeometryObject*> objects;
};

#endif
