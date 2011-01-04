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
#include "Cylinder.h"
#include "Disk.h"
#include "Torus.h"
#include "Box.h"
#include "Annulus.h"
#include "Instance.h"
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
}

GeometryManager::~GeometryManager() {
   for(unsigned int i = 0; i < objects.size(); i++) {
      delete objects[i];
   }
   objects.clear();
}

void GeometryManager::loadObjects(string fname) {
   std::ifstream fp(fname.c_str());
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

GeometryObject* GeometryManager::createObject(string type, Hash* hash, bool addToList) {
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
   else if(type == "cylinder") {
      obj = new Cylinder();
   }
   else if(type == "disk") {
      obj = new Disk();
   }
   else if(type == "torus") {
      obj = new Torus();
   }
   else if(type == "box") {
      obj = new Box();
   }
   else if(type == "annulus") {
      obj = new Annulus();
   }
   else if(type == "instance") {
      obj = new Instance();
   }
   else {
      return NULL;
   }
   
   obj->setHash(hash);
   
   if(addToList) {
      objects.push_back(obj);
   }

   return obj;
}
