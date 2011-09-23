#include "MedianFilter.h"
#include "SDL_Utility.h"
#include <algorithm>

void Slice::set(SDL_Surface* surface, int w, int h) {
   pixels[0] = get_pixel(surface, w, h-1);
   pixels[1] = get_pixel(surface, w, h);
   pixels[2] = get_pixel(surface, w, h+1);
   sort(pixels, pixels+3);
}

MedianFilter::MedianFilter() {
}

MedianFilter::~MedianFilter() {
}

SDL_Surface* MedianFilter::filter(SDL_Surface* surface) {
   Slice slices[4];
   vector<Uint32> twoThree(6);
   Uint32 finalMerge[7];
   
   SDL_Rect rect;
   rect.x = rect.y = 0;
   rect.w = surface->w;
   rect.h = surface->h;
   SDL_Surface* newSurface = createSurface(rect);
   
   Uint32 color = SDL_MapRGBA(newSurface->format, 0, 0, 255, 255);
   SDL_FillRect(newSurface, &rect, color);
   Uint8 r, g, b;
   
   for(int h = 1; h < surface->h - 1; h++) {
      int sidx = 0;
      slices[0].set(surface, 0, h);
      slices[1].set(surface, 1, h);

      for(int w = 1; w < surface->w - 2; w += 2) {
         int idx0 = sidx % 4;
         int idx1 = (sidx+1) % 4;
         int idx2 = (sidx+2) % 4;
         int idx3 = (sidx+3) % 4;

         // Step 1
         slices[idx2].set(surface, w+1, h);
         slices[idx3].set(surface, w+2, h);
         
         // Step 2 - Merge second and third slices two form sorted 6 item set
         merge(slices[idx1].pixels, slices[idx1].pixels + 3, slices[idx2].pixels, slices[idx2].pixels + 3, twoThree.begin());
         
         // Step 3a - Merge first slice with middle 4 items of sorted 6 set
         merge(slices[idx0].pixels, slices[idx0].pixels + 3, twoThree.begin() + 1, twoThree.begin() + 4, finalMerge);
         
         SDL_GetRGB(finalMerge[3], surface->format, &r, &g, &b);
         setPixel(newSurface, w, h, r, g, b);

         // Step 3b - Merge fourth slice with middle 4 items of sorted 6 set
         merge(slices[idx3].pixels, slices[idx3].pixels + 3, twoThree.begin() + 1, twoThree.begin() + 4, finalMerge);
         SDL_GetRGB(finalMerge[3], surface->format, &r, &g, &b);
         setPixel(newSurface, w+1, h, r, g, b);

         sidx += 2;
      }
   }

   return newSurface;
}
