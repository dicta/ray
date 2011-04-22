#include "ImageTexture.h"
#include "Parser/Hash.h"
#include <string>
#include <SDL_Image.h>

using namespace std;

ImageTexture::~ImageTexture() {
   SDL_FreeSurface(surf);
}

void ImageTexture::setHash(Hash* hash) {
   string fname = hash->getString("filename");
   surf = IMG_Load(fname.c_str());
   if(!surf) {
      printf("IMG_Load: %s\n", IMG_GetError());
      exit(1);
   }
   
   string mappingName = hash->getString("mapping");
   if(mappingName == "sphere") {
      mapping = SPHERE;
   }
   else if(mappingName == "disk") {
      mapping = DISK;
   }
   else if(mappingName == "uv") {
      mapping = UV;
   }
   else {
      assert(false);
   }
}

Color ImageTexture::getColor(const Point3D& p) const {
   double u, v, d;

   switch(mapping) {
      case SPHERE:
         u = (atan2(p.x, p.z) + M_PI) / (2.0 * M_PI);
         v = acos(p.y) / M_PI;
         break;

      case DISK:
         d = sqrt(p.x * p.x + p.z * p.z);
         u = (d - 1.236) / (2.326 - 1.236);
         v = 0;
         break;

      case UV:
         u = (p.x + surf->w/2.0) / surf->w;
         v = (p.y + surf->h/2.0) / surf->h;
         break;
   }

   int x = (int)(u * (surf->w - 1));
   int y = (int)(v * (surf->h - 1));
   Uint32 pixel = getpixel(surf, x, y);

   Uint8 r, g, b, a;
   SDL_GetRGBA(pixel, surf->format, &r, &g, &b, &a);
   return Color(r/255.0, g/255.0, b/255.0, a/255.0);
}

Uint32 ImageTexture::getpixel(SDL_Surface *surface, int x, int y) const {
   int bpp = surface->format->BytesPerPixel;
   /* Here p is the address to the pixel we want to retrieve */
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

   switch(bpp) {
   case 1: return *p;

   case 2: return *(Uint16 *)p;

   case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
         return p[0] << 16 | p[1] << 8 | p[2];
      }
      return p[0] | p[1] << 8 | p[2] << 16;

   case 4: return *(Uint32 *)p;

   default: return 0;       /* shouldn't happen, but avoids warnings */
   }
}
