#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <vector>
#include <string>
#include <SDL/SDL.h>
#include "Math/Point3D.h"
#include "Math/Vector3D.h"

using namespace std;

class Hash;
class Instance;
class Camera;

class FrameObject {

public:
   FrameObject(Instance* i);
   void setup();

   Instance* instance;
   Point3D position;
   Vector3D rotation;
};

struct Frame {
   vector<FrameObject*> objects;
};

struct CameraFrame {
   Point3D position;
   Vector3D rotation;
};

class Animation {

public:
   Animation(Camera* c, SDL_Surface* s);
   ~Animation();

   void setup(const string& fname);
   void play();

private:
   void loadAnimation(Hash* hash);
   void loadAnimationFrames(Array* frames, Instance* instance);
   void loadCameraFrames(Array* frames);

   unsigned frameCount;
   Camera* camera;
   SDL_Surface* surface;
   vector<Frame*> objFrames;
   vector<CameraFrame*> cameraFrames;
   string outputDir;
};

#endif
