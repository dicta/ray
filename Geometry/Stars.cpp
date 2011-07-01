#include "Stars.h"
#include "Sphere.h"
#include "Materials/Emissive.h"
#include "Geometry/GeometryManager.h"
#include "Parser/Hash.h"

Stars::Stars() : count(0), distance(0), baseSize(0) {
}

void Stars::setHash(Hash* h) {
   count = h->getInteger("count");
   distance = h->getDouble("distance");
   baseSize = distance / 1000.0;
}

void Stars::createStars() {
   srand(0);
   for(int i = 0; i < count; i++) {
      createStars(1, 1, 1);
      createStars(1, 1, -1);
      createStars(1, -1, 1);
      createStars(1, -1, -1);
      createStars(-1, 1, 1);
      createStars(-1, 1, -1);
      createStars(-1, -1, 1);
      createStars(-1, -1, -1);
   }
}

void Stars::createStars(int a, int b, int c) {
   float x = (float) rand() / (float) RAND_MAX * 10000.0;
   float y = (float) rand() / (float) RAND_MAX * 10000.0;
   float z = (float) rand() / (float) RAND_MAX * 10000.0;

   float r = (float) rand() / (float) RAND_MAX;

   float radius = (float) rand() / (float) RAND_MAX * baseSize;

   Vector3D v(x * a, y * b, z * c);
   v.normalize();
   v *= distance;

   Sphere* star = new Sphere(Point3D(v.x, v.y, v.z), radius);
   Emissive* em = new Emissive();
   em->setColor(max(0.0, r - 0.2), max(0.0, r - 0.2), r);
   em->setDiffuse(1.0);
   star->setMaterial(em);

   GeometryManager::instance().getGrid().addObject(star);
}
