#ifndef _SDL_UTILITY_H_
#define _SDL_UTILITY_H_

#include <SDL/SDL.h>
#include <png.h>
#include "Color.h"

void setPixel(SDL_Surface* s, int x, int y, const Color& color);
SDL_Surface* createSurface(const SDL_Rect& rect);

void user_error_fn(png_structp ctx, png_const_charp str);
void user_warning_fn(png_structp ctx, png_const_charp str);

int png_colortype_from_surface(SDL_Surface *surface);
void saveImage(SDL_Surface* surface);

#endif
