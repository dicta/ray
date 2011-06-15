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

FrameObject::FrameObject(Instance* i, int num) : instance(i), frameNum(num), rx(0), ry(0), rz(0) {
}

void FrameObject::setup() {
   instance->reset();
   instance->rotateX(rx);
   instance->rotateY(ry);
   instance->rotateZ(rz);
   instance->computeBBox();
}

Animation::Animation() : frames(NULL), frameCount(0), outputDir("") {
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
      else if(name == "animation") {
         loadAnimation(hash);
      }
   }
/*
   map<string, Instance*> instances;
   
   GeometryObject* obj = GeometryManager::instance().removeObject("stationV");
   Instance* instance = new Instance(obj);
   GeometryManager::instance().addObject(instance);
   instances["stationV"] = instance;

   for(int i = 0; i < frameCount; i++) {
      FrameObject* fo = new FrameObject(instances["stationV"]);
      fo->ry = ((double)i / (double)frameCount) * 20.0;
      frames[i].addFrameObject(fo);
   }
   
   instances.clear();
*/
}

void Animation::loadConfiguration(Hash* hash) {
   frameCount = hash->getInteger("frameCount");
   outputDir = hash->getString("outputDir");
   
   frames = new Frame[frameCount];
}

void Animation::loadAnimation(Hash* hash) {
   string objName = hash->getString("name");

   GeometryObject* obj = GeometryManager::instance().removeObject(objName);
   Instance* instance = new Instance(obj);
   GeometryManager::instance().addObject(instance);
   
   FrameObject* startFO = NULL;
   Array* frames = hash->getValue("frames")->getArray();
   for(ValueIter it = frames->begin(); it != frames->end(); ++it) {
      startFO = loadAnimationFrame((*it)->getHash(), instance, startFO);
   }
}

FrameObject* Animation::loadAnimationFrame(Hash* hash, Instance* instance, FrameObject* startFO) {
   int frameNum = hash->getInteger("number");
   int startFrame = 0;
   double srx = 0, sry = 0, srz = 0;
   
   if(startFO != NULL) {
      startFrame = startFO->frameNum;
      srx = startFO->rx;
      sry = startFO->ry;
   }
   
   double drx = srx, dry = sry, drz = srz;

   if(hash->contains("rx")) drx = hash->getDouble("rx");
   if(hash->contains("ry")) dry = hash->getDouble("ry");

   int frameCount = frameNum - startFrame;
   FrameObject* fo;

   for(int i = startFrame; i <= frameNum; i++) {
      fo = new FrameObject(instance, i);
      double p = (double)(i-startFrame) / (double)frameCount;
      if(frameCount == 0) p = 0;
      fo->rx = lerp<double>(p, srx, drx);
      fo->ry = lerp<double>(p, sry, dry);
      frames[i].addFrameObject(fo);
   }
   
   return fo;
}

void Animation::play() {
   char fname[512];
   for(int i = 0; i < frameCount; i++) {
      for(FrameObjectIter it = frames[i].objects.begin(); it != frames[i].objects.end(); ++it) {
         (*it)->setup();
         GeometryManager::instance().getGrid().setupCells();
         SDLApp::instance().getCamera()->render();
         sprintf(fname, "%s/image%d.bmp", outputDir.c_str(), i);
         SDLApp::instance().saveBMP(fname);
      }
   }
}
