#include "SDLApp.h"
#include "Math/Ray.h"
#include "Cameras/Pinhole.h"
#include "Cameras/ThinLens.h"
#include "Parser/Parser.h"
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
   
//   camera = new Pinhole(875);
//   camera = new ThinLens(875, 15);
//   camera->setEye(0, 0, 15);
//   camera->setLookat(0, 0, 0);
//   camera->setUp(0, 1, 0);
//   camera->setSurface(surface);
//   camera->computeUVW();
}

SDLApp::~SDLApp() {
   delete camera;
   SDL_Quit();
}

void SDLApp::loadConfiguration() {
   std::ifstream fin("config/config.txt", std::ios::in);
   Tokenizer tok(&fin);
   Parser parser(&tok);
   
   int width = 0;
   int height = 0;
   
   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }
      
      if(tok.getStringValue() == "config") {
         Hash *h = parser.readValue()->getHash();
         width = h->getInteger("width");
         height = h->getInteger("height");
      }
      else if(tok.getStringValue() == "camera") {
         Hash *h = parser.readValue()->getHash();
         setupCamera(h);
      }
   }
   
   surface = SDL_SetVideoMode(width, height, bpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
   if (surface == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
   
   camera->setWidth(width);
   camera->setHeight(height);
   camera->setSurface(surface);
   camera->computeUVW();
}

void SDLApp::setupCamera(Hash* h) {
   string type = h->getValue("type")->getString();
   if(type == "pinhole") {
      camera = new Pinhole();
   }
   else if(type == "thinLens") {
      camera = new ThinLens();
   }
   camera->setHash(h);
}

void SDLApp::run() {
   SDL_Event event;
   
   Uint32 start = SDL_GetTicks();
   
//   ViewPlane vp(width, height, scale);

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

