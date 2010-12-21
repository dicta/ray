#ifndef _SDLAPP_H_
#define _SDLAPP_H_

#include <SDL/SDL.h>
#include <string>

using namespace std;

class Camera;

class SDLApp {
   
public:
   SDLApp();
   ~SDLApp();
   void run();
   
private:
   void loadConfiguration();
   void setupCamera(string fname);

   bool stopApp;
   SDL_Surface* surface;
   Camera* camera;
};

#endif
