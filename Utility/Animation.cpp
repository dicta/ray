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

CameraFrame::CameraFrame() : frameNum(0), rx(0), ry(0) {
}

void CameraFrame::setup() {
   SDLApp::instance().getCamera()->rotate(rx, ry, 0);
}

Animation::Animation() : frames(NULL), cameraFrames(NULL), frameCount(0), outputDir("") {
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
}

void Animation::loadConfiguration(Hash* hash) {
   frameCount = hash->getInteger("frameCount");
   outputDir = hash->getString("outputDir");
   
   frames = new Frame[frameCount];
   cameraFrames = new CameraFrame[frameCount];
}

void Animation::loadAnimation(Hash* hash) {
   string objName = hash->getString("name");
   if(objName == "camera") {
      Array* frames = hash->getValue("frames")->getArray();
      int startFrame = 0;

      for(ValueIter it = frames->begin(); it != frames->end(); ++it) {
         startFrame = loadCameraFrame((*it)->getHash(), cameraFrames[startFrame]);
      }
   }
   else {
      GeometryObject* obj = GeometryManager::instance().removeObject(objName);
      Instance* instance = new Instance(obj);
      GeometryManager::instance().getGrid().addObject(instance);
   
      FrameObject* startFO = NULL;
      Array* frames = hash->getValue("frames")->getArray();
      for(ValueIter it = frames->begin(); it != frames->end(); ++it) {
         startFO = loadAnimationFrame((*it)->getHash(), instance, startFO);
      }
   }
}

FrameObject* Animation::loadAnimationFrame(Hash* hash, Instance* instance, FrameObject* startFO) {
   int frameNum = hash->getInteger("number");
   int startFrame = 0;
   double srx = 0, sry = 0, srz = 0;
   int loopStart = 0;

   if(startFO != NULL) {
      startFrame = startFO->frameNum;
      loopStart = startFrame + 1;
      srx = startFO->rx;
      sry = startFO->ry;
   }
   
   double drx = srx, dry = sry, drz = srz;

   if(hash->contains("rx")) drx = hash->getDouble("rx");
   if(hash->contains("ry")) dry = hash->getDouble("ry");

   FrameObject* fo;
   int frameCount = frameNum - startFrame;

   for(int i = loopStart; i <= frameNum; i++) {
      fo = new FrameObject(instance, i);
      double p = (double)(i-startFrame) / (double)frameCount;
      if(frameCount == 0) p = 0;
      fo->rx = lerp<double>(p, srx, drx);
      fo->ry = lerp<double>(p, sry, dry);
      frames[i].objects.push_back(fo);
   }

   return fo;
}

int Animation::loadCameraFrame(Hash* hash, const CameraFrame& startFO) {
   int frameNum = hash->getInteger("number");
   int startFrame = 0;
   double srx = 0, sry = 0, srz = 0;
   int loopStart = 0;

   if(startFO.frameNum > 0) {
      startFrame = startFO.frameNum;
      loopStart = startFrame + 1;
      srx = startFO.rx;
      sry = startFO.ry;
   }

   double drx = srx, dry = sry, drz = srz;

   if(hash->contains("rx")) drx = hash->getDouble("rx");
   if(hash->contains("ry")) dry = hash->getDouble("ry");

   int frameCount = frameNum - startFrame;

   for(int i = loopStart; i <= frameNum; i++) {
      double p = (double)(i-startFrame) / (double)frameCount;
      if(frameCount == 0) p = 0;
      cameraFrames[i].frameNum = i;
      cameraFrames[i].rx = lerp<double>(p, srx, drx);
      cameraFrames[i].ry = lerp<double>(p, sry, dry);
   }

   return frameNum;
}

void Animation::play() {
   char fname[512];
   for(int i = 0; i < frameCount; i++) {
      cameraFrames[i].setup();
      for(FrameObjectIter it = frames[i].objects.begin(); it != frames[i].objects.end(); ++it) {
         (*it)->setup();
         GeometryManager::instance().getGrid().setupCells();
         printf("Frame %d\t", i);
         SDLApp::instance().getCamera()->render();
         sprintf(fname, "%s/image%d.bmp", outputDir.c_str(), i);
         SDLApp::instance().saveBMP(fname);
      }
   }
   
   printf("Animation done\n");
}
