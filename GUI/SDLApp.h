#ifndef _SDLAPP_H_
#define _SDLAPP_H_

#include <string>

using namespace std;

class Camera;
class SDL_Surface;
class Animation;

class SDLApp {
   
public:
   SDLApp();
   ~SDLApp();
   void run();
   
   Camera* getCamera() { return camera; }
   
private:
   void loadConfiguration();
   void setupCamera(string fname, int width, int height);

   bool stopApp;
   SDL_Surface* surface;
   Camera* camera;
   Animation* animation;
};

#endif
