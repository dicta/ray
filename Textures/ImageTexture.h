#ifndef _IMAGE_TEXTURE_H_
#define _IMAGE_TEXTURE_H_

#include <SDL/SDL.h>
#include "Texture.h"

enum MappingEnum {
   UV,
   SPHERE,
   DISK
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
   virtual ~ImageTexture();

   virtual Color getColor(const ShadeRecord& sr) const { return getColor(sr.localHitPoint); }
   virtual void setHash(Hash* hash);
   virtual float getAlpha(const Point3D& p) const { return getColor(p).alpha; }

private:
   Uint32 getpixel(SDL_Surface *surface, int x, int y) const;
   Color getColor(const Point3D& p) const;

   SDL_Surface* surf;
   MappingEnum mapping;
};

#endif
