#include "SDL_Utility.h"

void setPixel(SDL_Surface* s, int x, int y, const Color& color) {
   int bpp = s->format->BytesPerPixel;
   /* Here p is the address to the pixel we want to set */
   Uint8 *p = (Uint8 *)s->pixels + y * s->pitch + x * bpp;
   Uint32 pixel = SDL_MapRGBA(s->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
   *(Uint32 *)p = pixel;
}

Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
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

SDL_Surface* createSurface(const SDL_Rect& rect) {
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

void user_error_fn(png_structp ctx, png_const_charp str) {
   printf("libpng error %s\n", str);
}

void user_warning_fn(png_structp ctx, png_const_charp str) {
   printf("libpng warning %s\n", str);
}

int png_colortype_from_surface(SDL_Surface *surface) {
   int colortype = PNG_COLOR_MASK_COLOR; /* grayscale not supported */

	if (surface->format->palette)
		colortype |= PNG_COLOR_MASK_PALETTE;
	else if (surface->format->Amask)
		colortype |= PNG_COLOR_MASK_ALPHA;

	return colortype;
}

void saveImage(SDL_Surface* surface, const char* fname) {
   png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, user_error_fn, user_warning_fn);
   if(png_ptr == NULL) {
      printf("Unable to create png_ptr\n");
      return;
   }

   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL) {
      printf("Unable to create info_ptr\n");
      png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
      return;
   }

   // Setup error handling
   if (setjmp(png_jmpbuf(png_ptr))) {
      printf("An error has occured\n");
      png_destroy_write_struct(&png_ptr, &info_ptr);
      return;
   }

   // Setup the output code
   FILE* fp = fopen(fname, "wb");
   png_init_io(png_ptr, fp);

   int colortype = png_colortype_from_surface(surface);
   png_set_IHDR(png_ptr, info_ptr, surface->w, surface->h, 8,
                colortype, // color type
                PNG_INTERLACE_NONE, // interlace type
                PNG_COMPRESSION_TYPE_DEFAULT, // compression type
                PNG_FILTER_TYPE_DEFAULT); // filter method

   png_set_bgr(png_ptr);

   /* Writing the image */
   png_write_info(png_ptr, info_ptr);
   png_set_packing(png_ptr);

   png_bytep *row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * surface->h);
   for (int i = 0; i < surface->h; i++) {
      row_pointers[i] = (png_bytep)(Uint8 *)surface->pixels + i * surface->pitch;
   }

   png_write_image(png_ptr, row_pointers);
   png_write_end(png_ptr, info_ptr);

   /* Cleaning out... */
   free(row_pointers);
   png_destroy_write_struct(&png_ptr, &info_ptr);
   fclose(fp);
}
