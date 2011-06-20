#include "Animation.h"
#include "Parser/Parser.h"
#include <iostream>
#include <fstream>
#include "Geometry/GeometryManager.h"
#include "Geometry/GeometryObject.h"
#include "Geometry/Instance.h"
#include "Math/Maths.h"
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

CameraFrame::CameraFrame() : frameNum(0), rx(0), ry(0), rz(0) {
}

Animation::Animation(Camera* c, SDL_Surface* s) : camera(c), surface(s), frames(NULL), cameraFrames(NULL), frameCount(0), outputDir("") {
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

   if(frameNum == 0) {
      if(hash->contains("rx")) cameraFrames[0].rx = hash->getDouble("rx");
      if(hash->contains("ry")) cameraFrames[0].ry = hash->getDouble("ry");
      if(hash->contains("rz")) cameraFrames[0].rz = hash->getDouble("rz");
      return 0;
   }
   
   int loopStart = startFO.frameNum + 1;

   double srx = startFO.rx;
   double sry = startFO.ry;
   double srz = startFO.rz;

   double drx = srx, dry = sry, drz = srz;

   if(hash->contains("rx")) drx = hash->getDouble("rx");
   if(hash->contains("ry")) dry = hash->getDouble("ry");
   if(hash->contains("rz")) drz = hash->getDouble("rz");

   int frameCount = frameNum - startFO.frameNum;

   for(int i = loopStart; i <= frameNum; i++) {
      double p = (double)(i-startFO.frameNum) / (double)frameCount;
      if(frameCount == 0) p = 0;
      cameraFrames[i].frameNum = i;
      cameraFrames[i].rx = lerp<double>(p, srx, drx);
      cameraFrames[i].ry = lerp<double>(p, sry, dry);
      cameraFrames[i].rz = lerp<double>(p, srz, drz);
   }

   return frameNum;
}

void Animation::play() {
   char fname[512];
   for(int i = 0; i < frameCount; i++) {
      camera->rotate(cameraFrames[i].rx, cameraFrames[i].ry, cameraFrames[i].rz);
      for(FrameObjectIter it = frames[i].objects.begin(); it != frames[i].objects.end(); ++it) {
         (*it)->setup();
         GeometryManager::instance().getGrid().setupCells();
         printf("Frame %d\t", i);
         camera->render();
         sprintf(fname, "%s/image%d.bmp", outputDir.c_str(), i);
         SDL_SaveBMP(surface, fname);
      }
   }
   
   printf("Animation done\n");
}
