#include "MedianFilter.h"
#include "SDL_Utility.h"
#include <algorithm>

Slice::Slice(Uint32 a, Uint32 b, Uint32 c) {
   pixels[0] = a;
   pixels[1] = b;
   pixels[2] = c;
   sort(pixels, pixels+3);
}

MedianFilter::MedianFilter() {
}

MedianFilter::~MedianFilter() {
}

void MedianFilter::filter(SDL_Surface* surface) {
   for(int h = 1; h < surface->h-1; h++) {
      for(int w = 1; w < surface->w-1; w += 2) {
         Uint32 a = get_pixel(surface, w, h-1);
         Uint32 b = get_pixel(surface, w, h);
         Uint32 c = get_pixel(surface, w, h+1);
         Slice slice(a, b, c);
      }
   }

}
