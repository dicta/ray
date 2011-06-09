#ifndef _SDLAPP_H_
#define _SDLAPP_H_

#include <string>
#include <memory>

using namespace std;

class Camera;
class SDL_Surface;

class SDLApp {
   
public:
   static SDLApp& instance();
   ~SDLApp();
   void run();
   
   Camera* getCamera() { return camera; }
   void saveBMP(const char* fname);
   
private:
   SDLApp();
   void loadConfiguration();
   void setupCamera(string fname, int width, int height);

   static auto_ptr<SDLApp> s_instance;
   bool stopApp;
   SDL_Surface* surface;
   Camera* camera;
};

#endif
