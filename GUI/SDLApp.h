#ifndef _SDLAPP_H_
#define _SDLAPP_H_

#include <SDL/SDL.h>

class Camera;
class Hash;

class SDLApp {
   
public:
   SDLApp();
   ~SDLApp();
   void run();
   
private:
   void loadConfiguration();
   void setupCamera(Hash* h);

   bool stopApp;
   SDL_Surface* surface;
   Camera* camera;
};

#endif
