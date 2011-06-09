#include "Animation.h"
#include "Parser/Parser.h"
#include <iostream>
#include <fstream>
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Geometry/Instance.h"
#include "Math/Maths.h"
#include "GUI/SDLApp.h"
#include "Cameras/Camera.h"

typedef vector<FrameObject*>::const_iterator FrameObjectIter;

FrameObject::FrameObject(Instance* i) : instance(i), rx(0), ry(0), rz(0) {
}

void FrameObject::setup() {
}

Animation::Animation() : frames(NULL), frameCount(0) {
}

Animation::~Animation() {
   delete[] frames;
}

void Animation::setup(const string& fname) {
   ifstream fp(fname.c_str());
   Tokenizer tok(&fp);
   Parser parser(&tok);

   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }

      string name = tok.getStringValue();
      Hash* hash = parser.readValue()->getHash();

      if(name == "config") {
         loadConfiguration(hash);
      }
   }
   
   frames = new Frame[frameCount];
   map<string, Instance*> instances;
   
   GeometryObject* obj = GeometryManager::instance().removeObject("stationV");
   Instance* instance = new Instance(obj);
   GeometryManager::instance().addObject(instance);
   instances["stationV"] = instance;

   for(int i = 0; i < frameCount; i++) {
      FrameObject* fo = new FrameObject(instances["stationV"]);
      fo->ry = lerp<double>(i, 0, 20);
      frames[i].addFrameObject(fo);
   }
   
   instances.clear();
}

void Animation::loadConfiguration(Hash* hash) {
   frameCount = hash->getInteger("frameCount");
}

void Animation::play() {
   char fname[512];
   for(int i = 0; i < frameCount; i++) {
      for(FrameObjectIter it = frames[i].objects.begin(); it != frames[i].objects.end(); ++it) {
         (*it)->setup();
         GeometryManager::instance().getGrid().setupCells();
         SDLApp::instance().getCamera()->render();
         sprintf(fname, "output/image%d.bmp", i);
         SDLApp::instance().saveBMP(fname);
      }
   }
}
