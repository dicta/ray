/*
 *  LightManager.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_

#include <vector>
#include <memory>
#include "Utility/Color.h"
#include "Utility/ShadeRecord.h"

using namespace std;

class Light;
class Ambient;

typedef vector<Light*>::const_iterator LightIter;

class LightManager {
   
public:
   static LightManager& instance();
   ~LightManager();
   
   LightIter begin() const { return lights.begin(); }
   LightIter end() const { return lights.end(); }
   Color getAmbientLight(const ShadeRecord& sr) const;

private:
   LightManager();
   void loadLights();
   
   static auto_ptr<LightManager> s_instance;
   vector<Light*> lights;
   Ambient* ambient;
};

#endif
