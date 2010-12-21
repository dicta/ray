/*
 *  LightManager.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "LightManager.h"
#include "PositionLight.h"
#include "Direction.h"
#include "AmbientOccluder.h"
#include "AreaLight.h"
#include "Environment.h"
#include "Parser/Parser.h"
#include <fstream>

auto_ptr<LightManager> LightManager::s_instance;

LightManager& LightManager::instance() {
   if(s_instance.get() == 0) {
      s_instance.reset(new LightManager());
   }
   return *s_instance;
}

LightManager::LightManager() : ambient(NULL) {
}

LightManager::~LightManager() {
   for(unsigned int i = 0; i < lights.size(); i++) {
      delete lights[i];
   }
   lights.clear();

   delete ambient;
}

void LightManager::loadLights(string fname) {
   std::ifstream fp(fname.c_str());
   Tokenizer tok(&fp);
   Parser parser(&tok);

   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }

      Light* light = NULL;
      string type = tok.getStringValue();
      Hash* hash = parser.readValue()->getHash();

      if(type == "ambient") {
         ambient = new Ambient();
         ambient->setHash(hash);
      }
      else if(type == "ambientOccluder") {
         ambient = new AmbientOccluder();
         ambient->setHash(hash);
      }
      else if(type == "position") {
         light = new PositionLight();
      }
      else if(type == "direction") {
         light = new Direction();
      }
      else if(type == "areaLight") {
         light = new AreaLight();
      }
      else if(type == "environment") {
         light = new Environment();
      }
      else {
         continue;
      }

      if(light != NULL) {
         light->setHash(hash);
         lights.push_back(light);
      }
   }

   fp.close();
}

Color LightManager::getAmbientLight(const ShadeRecord& sr) const {
   if(ambient != NULL) {
      return ambient->L(sr);
   }
   return BLACK;
}
