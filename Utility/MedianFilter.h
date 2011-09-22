#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <SDL/SDL.h>

struct Slice {
   Slice(Uint32 a, Uint32 b, Uint32 c);
   Uint32 pixels[3];
};

class MedianFilter
{
public:
   MedianFilter();
   virtual ~MedianFilter();

   void filter(SDL_Surface* surface);

private:
};

#endif // MEDIANFILTER_H
