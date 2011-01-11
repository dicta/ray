#include "MeditParser.h"
#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <algorithm>

MeditParser::MeditParser() : Mesh() {
}

void MeditParser::readMesh(string fname) {
   ifstream fp(fname.c_str(), ios::in);
   fprintf(stderr, "failbit = %d\n", fp.fail());
   string line;

//   while(line != "Vertices") {
//      fp >> line;
//   }

   int count;
   double x, y, z;
   int i;
   fp >> count;

   for (int vidx = 0; vidx < count; vidx++) {
      fp >> x >> y >> z >> i;
      addPoint(new Point3D(x, y, z));
   }

   std::string shape;
   fp >> shape;
   bool done = false;

   while (!done) {
      if (shape == "End") {
         done = true;
      } else {
         fp >> count;
         if (shape == "Triangles") {
            readTriangles(count, fp);
         } else if (shape == "Quadrilaterals") {
         }
      }
      fp >> shape;
   }

   fp.close();

   calculateNormals();
}

void MeditParser::readTriangles(int count, ifstream& fp) {
   int idx, idx1, idx2, idx3;
   for (int i = 0; i < count; i++) {
      fp >> idx1 >> idx2 >> idx3;
      // Skip last number
      fp >> idx;

      idx1--;
      idx2--;
      idx3--;
      addFace(new Face(idx1, idx2, idx3));
   }
}
