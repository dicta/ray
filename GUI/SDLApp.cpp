#include "SDLApp.h"
#include "Math/Ray.h"
#include "Cameras/Pinhole.h"
#include "Cameras/ThinLens.h"
#include "Parser/Parser.h"
#include "Lights/LightManager.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/Mesh/MeshManager.h"
#include "Utility/Animation.h"
#include <math.h>
#include <fstream>
#include <SDL/SDL_image.h>
#include "Utility/SDL_Utility.h"

SDLApp::SDLApp(int argc, char** argv) :stopApp(false), surface(NULL), camera(NULL), animation(NULL) {
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }

   IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

   loadConfiguration(argc, argv);
}

SDLApp::~SDLApp() {
   delete camera;
   SDL_FreeSurface(surface);

   if(animation != NULL) {
      delete animation;
   }

   IMG_Quit();
   SDL_Quit();
}

void SDLApp::loadConfiguration(int argc, char** argv) {
   const char* config_file = "config/config.txt";
   if (argc > 1) {
      config_file = argv[1];
   }
   std::ifstream fin(config_file, std::ios::in);
   
   Tokenizer tok(&fin);
   Parser parser(&tok);

   tok.nextToken();
   Hash *h = parser.readValue()->getHash();
   int width = h->getInteger("width");
   int height = h->getInteger("height");

   int threadCount = h->getInteger("threads");
   int boxw = h->getInteger("boxWidth");
   int boxh = h->getInteger("boxHeight");

   if(h->contains("maxGridCells")) {
      int maxCells = h->getInteger("maxGridCells");
      GeometryManager::instance().setMaxGridCells(maxCells);
   }

   surface = SDL_SetVideoMode(width, height, 24, SDL_HWSURFACE | SDL_DOUBLEBUF);
   if (surface == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}

   setupCamera(h->getString("camera"), width, height);
   camera->setSurface(surface);
   camera->setThreadParameters(threadCount, boxw, boxh);

   if(h->contains("mesh")) {
      MeshManager::instance().loadMeshes(h->getString("mesh"));
   }

   LightManager::instance().loadLights(h->getString("lights"));
   GeometryManager::instance().loadObjects(h->getString("objects"));

   if(h->contains("animation")) {
      animation = new Animation(camera, surface);
      animation->setup(h->getString("animation"));
   }
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

   if(animation != NULL) {
      animation->play();
   }
   else {
      camera->render();
   }

   while(!stopApp) {
      SDL_WaitEvent(&event);
      switch (event.type) {
         case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE) {
               stopApp = true;
            }
            else if(event.key.keysym.sym == 's') {
               saveImage(surface, "image.png");
            }
            break;

         case SDL_QUIT:
            stopApp = true;
            break;
      }
   }
}
