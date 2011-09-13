#include "StarBox.h"
#include "Geometry/GeometryManager.h"
#include "Geometry/Rectangle.h"
#include "Parser/Hash.h"
#include "Utility/SDL_Utility.h"
#include "Textures/ImageTexture.h"

StarBox::StarBox() : count(0), halfSize(1), size(2) {
}

void StarBox::setHash(Hash* h) {
   count = h->getInteger("count");
   halfSize = h->getInteger("halfSize");
   size = halfSize * 2;
}

void StarBox::createStars() {
   srand(0);
   createBoxSide("yzNegX", Point3D(-halfSize, -halfSize, -halfSize), Vector3D(0.0, size, 0.0), Vector3D(0.0, 0.0, size));
   createBoxSide("yzPosX", Point3D(halfSize, -halfSize, -halfSize), Vector3D(0.0, 0.0, size), Vector3D(0.0, size, 0.0));
   createBoxSide("xyNegZ", Point3D(-halfSize, -halfSize, -halfSize), Vector3D(size, 0.0, 0.0), Vector3D(0.0, size, 0.0));
   createBoxSide("xyPosZ", Point3D(-halfSize, -halfSize, halfSize), Vector3D(0.0, size, 0.0), Vector3D(size, 0.0, 0.0));
   createBoxSide("xzNegY", Point3D(-halfSize, -halfSize, -halfSize), Vector3D(0.0, 0.0, size), Vector3D(size, 0.0, 0.0));
   createBoxSide("xzPosY", Point3D(-halfSize, halfSize, -halfSize), Vector3D(size, 0.0, 0.0), Vector3D(0.0, 0.0, size));
}

void StarBox::createBoxSide(string name, const Point3D& origin, const Vector3D& a, const Vector3D& b) {
   SDL_Rect srect;
   srect.w = srect.h = size;
   SDL_Surface* surface = createSurface(srect);

   for(int i = 0; i < count; i++) {
      float x = (float) rand() / (float) RAND_MAX * (size - 1);
      float y = (float) rand() / (float) RAND_MAX * (size - 1);
      float c = (float) rand() / (float) RAND_MAX;
      setPixel(surface, x, y, Color(c, c, c));
   }

   ImageTexture* texture = new ImageTexture();
   texture->setSurface(surface);

   shared_ptr<Emissive> em(new Emissive());
   em->setTexture(texture);

   Rectangle* rect = new Rectangle(origin, a, b);
   rect->setMaterial(em);
   rect->ignoreShadow = true;
   GeometryManager::instance().getGrid().addObject(rect);
}
