#ifndef _SDLAPP_H_
#define _SDLAPP_H_

#include <SDL/SDL.h>
#include <string>
#include "utility/Color.h"

using namespace std;

class Camera;
class Animation;

class SDLApp {

public:
   SDLApp(int argc, char** argv);
   ~SDLApp();
   void run();

   Camera* getCamera() { return camera; }

private:
   void loadConfiguration(int argc, char** argv);
   void setupCamera(string fname, int width, int height);

   bool stopApp;
   SDL_Surface* surface;
   Camera* camera;
   Animation* animation;
};

#endif
