#include "Stars.h"
#include "Sphere.h"
#include "Materials/Emissive.h"
#include "Geometry/GeometryManager.h"
#include "Parser/Hash.h"
#include "Math/Maths.h"
#include <fstream>

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
/*
   std::ifstream fp("config/stars.txt");
   int id;
   double theta, phi, dist, mag;
   string claz;

   fp >> id;
   while(!fp.eof()) {
      fp >> id >> theta >> phi >> dist >> mag >> claz;
      double x = cos(theta * DEG_TO_RAD) * sin(phi * DEG_TO_RAD);
      double y = sin(theta * DEG_TO_RAD) * sin(phi * DEG_TO_RAD);
      double z = cos(phi * DEG_TO_RAD);
      Point3D v(x, y, z);
      v *= distance;

//      float radius = (float) rand() / (float) RAND_MAX * baseSize;

      Sphere* star = new Sphere(v, mag * 0.1);
      shared_ptr<Emissive> em(new Emissive());

      if(claz[0] == 'O') em->setColor(0.6, 0.686, 1.0);
      else if(claz[0] == 'B') em->setColor(0.79, 0.84, 1.0);
      else if(claz[0] == 'A') em->setColor(1.0, 1.0, 1.0);
      else if(claz[0] == 'F') em->setColor(0.989, 1.0, 0.827);
      else if(claz[0] == 'G') em->setColor(1.0, 0.949, 0.631);
      else if(claz[0] == 'K') em->setColor(1.0, 0.639, 0.317);
      else if(claz[0] == 'M') em->setColor(1.0, 0.38, 0.317);
      else em->setColor(0.5, 0.5, 0.5);

      em->setDiffuse(1.0);
      star->setMaterial(em);
      star->ignoreShadow = true;

      GeometryManager::instance().getGrid().addObject(star);
   }

   printf("stars done\n");
*/
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
   shared_ptr<Emissive> em(new Emissive());
   r = max(0.0, r - 0.2);
   em->setColor(r, r, r);
   em->setDiffuse(1.0);
   star->setMaterial(em);
   star->ignoreShadow = true;

   GeometryManager::instance().getGrid().addObject(star);
}
