#include "Stars.h"
#include "Sphere.h"
#include "Materials/Emissive.h"
#include "Geometry/GeometryManager.h"
#include "Parser/Hash.h"

Stars::Stars() : count(0) {
}

void Stars::setHash(Hash* h) {
   count = h->getInteger("count");
}

void Stars::createStars() {
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

   float radius = (float) rand() / (float) RAND_MAX * 0.1;

   Vector3D v(x * a, y * b, z * c);
   v.normalize();
   v *= 100.0;
   
   Sphere* star = new Sphere(Point3D(v.x, v.y, v.z), radius);
   Emissive* em = new Emissive();
   em->setColor(max(0.0, r - 0.2), max(0.0, r - 0.2), r);
   em->setDiffuse(1.0);
   star->setMaterial(em);
   
   GeometryManager::instance().addObject(star);
}
