#ifndef _IMAGE_TEXTURE_H_
#define _IMAGE_TEXTURE_H_

#include <SDL/SDL.h>
#include "Texture.h"

enum MappingEnum {
   UV,
   SPHERE
};

/**
 * Creates a texture from an image file.
 *
 * Hash values:
 * <ul>
 * <li>filename <i>string</i> - Name of image file to use for texture
 * <li>mapping <i>string</i> - Mapping to use for generating texture coordinates. Possible values are sphere, disk, uv.
 * </ul>
 */
class ImageTexture : public Texture {

public:
   ImageTexture();
   virtual ~ImageTexture();

   virtual Color getColor(const ShadeRecord& sr) const;
   void setTextureFile(string fname);
   virtual void setHash(Hash* hash);
   virtual float getAlpha(const ShadeRecord& sr) const;
   void setSurface(SDL_Surface* s) { surf = s; }

private:
   Uint32 getPixel(const ShadeRecord& sr) const;
   Uint32 getpixel(SDL_Surface *surface, int x, int y) const;

   SDL_Surface* surf;
   MappingEnum mapping;
   bool colorAsAlpha;
};

#endif
