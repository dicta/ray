#ifndef _SDLAPP_H_
#define _SDLAPP_H_

#include <string>
#include <SDL/SDL.h>
#include "utility/Color.h"

using namespace std;

class Camera;
class Animation;

class SDLApp {
   
public:
   SDLApp();
   ~SDLApp();
   void run();
   
   Camera* getCamera() { return camera; }
   static SDL_Surface* createSurface(const SDL_Rect& rect);
   static void setPixel(SDL_Surface* s, int x, int y, const Color& color);
   
private:
   void loadConfiguration();
   void setupCamera(string fname, int width, int height);

   bool stopApp;
   SDL_Surface* surface;
   Camera* camera;
   Animation* animation;
};

#endif
