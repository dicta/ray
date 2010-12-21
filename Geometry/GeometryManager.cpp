/*
 *  GeometryManager.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <fstream>
#include "GeometryManager.h"
#include "GeometryObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "Rectangle.h"
#include "parser/Parser.h"
#include "Materials/Matte.h"

auto_ptr<GeometryManager> GeometryManager::s_instance;

GeometryManager& GeometryManager::instance() {
   if(s_instance.get() == 0) {
      s_instance.reset(new GeometryManager());
   }
   return *s_instance;
}

GeometryManager::GeometryManager() {
   loadScene();
}

GeometryManager::~GeometryManager() {
   for(unsigned int i = 0; i < objects.size(); i++) {
      delete objects[i];
   }
   objects.clear();
}

void GeometryManager::loadScene() {
   std::ifstream fp("config/scene.txt");
   Tokenizer tok(&fp);
   Parser parser(&tok);

   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }
      
      string type = tok.getStringValue();
      createObject(type, parser.readValue()->getHash());
   }
   
   fp.close();
}

GeometryObject* GeometryManager::createObject(string type, Hash* hash) {
   GeometryObject* obj;

   if(type == "sphere") {
      obj = new Sphere();
   }
   else if(type == "plane") {
      obj = new Plane();
   }
   else if(type == "rectangle") {
      obj = new Rectangle();
   }
   else {
      return NULL;
   }
   
   obj->setHash(hash);
   objects.push_back(obj);
   return obj;
}
