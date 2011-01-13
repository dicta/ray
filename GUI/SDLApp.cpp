#include "SDLApp.h"
#include "Math/Ray.h"
#include "Cameras/Pinhole.h"
#include "Cameras/ThinLens.h"
#include "Parser/Parser.h"
#include "Lights/LightManager.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/Mesh/MeshManager.h"
#include <math.h>
#include <fstream>

const int bpp = 0;

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
   
   int threadCount = h->getInteger("threads");
   int boxw = h->getInteger("boxWidth");
   int boxh = h->getInteger("boxHeight");
   
   surface = SDL_SetVideoMode(width, height, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
   if (surface == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
   
   setupCamera(h->getString("camera"), width, height);
   camera->setSurface(surface);
   camera->computeUVW();
   camera->setThreadParameters(threadCount, boxw, boxh);
   
   if(h->contains("mesh")) {
      MeshManager::instance().loadMeshes(h->getString("mesh"));
   }

   LightManager::instance().loadLights(h->getString("lights"));
   GeometryManager::instance().loadObjects(h->getString("objects"));
}

void SDLApp::setupCamera(string fname, int width, int height) {
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
            camera = new Pinhole(width, height);
         }
         else if(type == "thinLens") {
            camera = new ThinLens(width, height);
         }
         camera->setHash(h);
      }
   }
}

void SDLApp::run() {
   SDL_Event event;
   camera->render();

   while(!stopApp) {
      SDL_WaitEvent(&event);
      switch (event.type) {
//         case SDL_KEYDOWN :
         case SDL_QUIT:
            stopApp = true;
            break;
      }
   }
}

