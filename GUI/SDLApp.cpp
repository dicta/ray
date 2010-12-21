#include "SDLApp.h"
#include "Math/Ray.h"
#include "Cameras/Pinhole.h"
#include "Cameras/ThinLens.h"
#include "Parser/Parser.h"
#include "Lights/LightManager.h"
#include "Geometry/GeometryManager.h"
#include <math.h>
#include <fstream>

//const int width = 1024;
//const int height = 768;
const int bpp = 0;
//const double scale = 1.0;

SDLApp::SDLApp() :stopApp(false) {
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

   loadConfiguration();
}

SDLApp::~SDLApp() {
   delete camera;
   SDL_Quit();
}

void SDLApp::loadConfiguration() {
   std::ifstream fin("config/config.txt", std::ios::in);
   Tokenizer tok(&fin);
   Parser parser(&tok);
   
   tok.nextToken();
   Hash *h = parser.readValue()->getHash();
   int width = h->getInteger("width");
   int height = h->getInteger("height");

   surface = SDL_SetVideoMode(width, height, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
   if (surface == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
   
   setupCamera(h->getString("camera"));
   camera->setWidth(width);
   camera->setHeight(height);
   camera->setSurface(surface);
   camera->computeUVW();
   
   LightManager::instance().loadLights(h->getString("lights"));
   GeometryManager::instance().loadObjects(h->getString("objects"));
}

void SDLApp::setupCamera(string fname) {
   std::ifstream fin(fname.c_str(), std::ios::in);
   Tokenizer tok(&fin);
   Parser parser(&tok);

   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }

      string token = tok.getStringValue();
      if(token == "camera") {
         Hash* h = parser.readValue()->getHash();
         string type = h->getValue("type")->getString();
         if(type == "pinhole") {
            camera = new Pinhole();
         }
         else if(type == "thinLens") {
            camera = new ThinLens();
         }
         camera->setHash(h);
      }
   }
}

void SDLApp::run() {
   SDL_Event event;
   
   Uint32 start = SDL_GetTicks();
   camera->renderScene();
   Uint32 stop = SDL_GetTicks();
   
   printf("Render time = %f seconds\n", (stop - start) / 1000.0);

   while(!stopApp) {
      while ( SDL_PollEvent(&event)) {
         switch (event.type) {
//            case SDL_KEYDOWN :
            case SDL_QUIT:
               stopApp = true;
               break;
         }
      }      
   }
}

