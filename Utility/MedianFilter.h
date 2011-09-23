#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <SDL/SDL.h>

struct Slice {
   void set(SDL_Surface* surface, int w, int h);
   Uint32 pixels[3];
};

class MedianFilter
{
public:
   MedianFilter();
   virtual ~MedianFilter();

   SDL_Surface* filter(SDL_Surface* surface);

private:
};

#endif // MEDIANFILTER_H
