#include "ImageTexture.h"
#include "Parser/Hash.h"
#include <string>
#include <SDL/SDL_Image.h>
#include "utility/SDL_Utility.h"

using namespace std;

ImageTexture::ImageTexture() : Texture(), surf(NULL), mapping(UV), colorAsAlpha(false) {
}

ImageTexture::~ImageTexture() {
   SDL_FreeSurface(surf);
}

void ImageTexture::setTextureFile(string fname) {
   surf = IMG_Load(fname.c_str());
   if(!surf) {
      printf("IMG_Load: %s\n", IMG_GetError());
      exit(1);
   }
}

void ImageTexture::setHash(Hash* hash) {
   setTextureFile(hash->getString("filename"));

   string mappingName = hash->getString("mapping");
   if(mappingName == "sphere") {
      mapping = SPHERE;
   }
   else if(mappingName == "uv") {
      mapping = UV;
   }
   else {
       exit(1);
   }

   if(hash->contains("colorAsAlpha")) {
      colorAsAlpha = true;
   }
}

Uint32 ImageTexture::getPixel(const ShadeRecord& sr) const {
   double u, v;
   Point3D p = sr.localHitPoint;

   switch(mapping) {
      case SPHERE:
         u = (atan2(p.x, p.z) + M_PI) / (2.0 * M_PI);
         v = acos(p.y) / M_PI;
         break;

      case UV:
         u = sr.tu; // (p.x + surf->w/2.0) / surf->w;
         v = sr.tv; // (p.y + surf->h/2.0) / surf->h;
         break;
   }

   int x = (int)(u * (surf->w - 1));
   int y = (int)(v * (surf->h - 1));

   return get_pixel(surf, x, y);
}

Color ImageTexture::getColor(const ShadeRecord& sr) const {
   Uint32 pixel = getPixel(sr);
   Uint8 r, g, b, a;
   SDL_GetRGBA(pixel, surf->format, &r, &g, &b, &a);
   if(colorAsAlpha) {
      a = r;
   }
   return Color(r/255.0, g/255.0, b/255.0, a/255.0);
}

float ImageTexture::getAlpha(const ShadeRecord& sr) const {
   Uint32 pixel = getPixel(sr);
   Uint8 r, g, b, a;
   SDL_GetRGBA(pixel, surf->format, &r, &g, &b, &a);
   if(colorAsAlpha) {
      a = r;
   }
   return a / 255.0;
}
