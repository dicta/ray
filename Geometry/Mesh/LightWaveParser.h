#ifndef _LIGHTWAVE_PARSER_H_
#define _LIGHTWAVE_PARSER_H_

#include "Mesh.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

class LightWaveParser : public Mesh {

public:
   LightWaveParser();

   void loadModel(string fname);

protected:
   virtual bool hitFace(Face* face, const Ray& ray, double& tmin, ShadeRecord& sr) const;

private:
   ifstream in;

   int parseTags();
   int parsePTag();
   int parsePoints();
   int parsePolygons();
   int parseSurface();
   int skipChunk();

   vector<string> tags;
   mutable map<string, Material*> materialMap;
};

#endif
