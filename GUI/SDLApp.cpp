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
//#include <SDL/SDL_image.h>

const int bpp = 0;

SDLApp::SDLApp() :stopApp(false), surface(NULL), camera(NULL), animation(NULL) {
   if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }

//   IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

   loadConfiguration();
}

SDLApp::~SDLApp() {
   delete camera;
   SDL_FreeSurface(surface);

   if(animation != NULL) {
      delete animation;
   }

//   IMG_Quit();
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
               SDL_SaveBMP(surface, "test.bmp");
            }
            break;

         case SDL_QUIT:
            stopApp = true;
            break;
      }
   }
}

SDL_Surface* SDLApp::createSurface(const SDL_Rect& rect) {
   SDL_Surface *surface;
   Uint32 rmask, gmask, bmask, amask;
   
   /* SDL interprets each pixel as a 32-bit number, so our masks must depend
    on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
   rmask = 0xff000000;
   gmask = 0x00ff0000;
   bmask = 0x0000ff00;
   amask = 0x000000ff;
#else
   rmask = 0x000000ff;
   gmask = 0x0000ff00;
   bmask = 0x00ff0000;
   amask = 0xff000000;
#endif
   
   surface = SDL_CreateRGBSurface(SDL_HWSURFACE, rect.w, rect.h, 32, rmask, gmask, bmask, amask);
   if(surface == NULL) {
      fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
      exit(1);
   }
   
   return surface;
}

void SDLApp::setPixel(SDL_Surface* s, int x, int y, const Color& color) {
   int bpp = s->format->BytesPerPixel;
   /* Here p is the address to the pixel we want to set */
   Uint8 *p = (Uint8 *)s->pixels + y * s->pitch + x * bpp;
   Uint32 pixel = SDL_MapRGBA(s->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
   *(Uint32 *)p = pixel;
}
