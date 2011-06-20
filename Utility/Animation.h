#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <vector>
#include <string>
#include <SDL/SDL.h>

using namespace std;

class Hash;
class Instance;
class Camera;

class FrameObject {

public:
   FrameObject(Instance* i, int num);
   void setup();

   Instance* instance;
   int frameNum;
   double rx, ry, rz;
};

struct Frame {
   vector<FrameObject*> objects;
};

class CameraFrame {

public:
   CameraFrame();
   
   int frameNum;
   double rx, ry, rz;
};

class Animation {

public:
   Animation(Camera* c, SDL_Surface* s);
   ~Animation();

   void setup(const string& fname);
   void play();

private:
   void loadConfiguration(Hash* hash);
   void loadAnimation(Hash* hash);
   FrameObject* loadAnimationFrame(Hash* hash, Instance* instance, FrameObject* startFO);
   int loadCameraFrame(Hash* hash, const CameraFrame& startFO);

   Camera* camera;
   SDL_Surface* surface;
   Frame* frames;
   CameraFrame* cameraFrames;
   int frameCount;
   string outputDir;
};

#endif
