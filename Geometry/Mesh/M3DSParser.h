#ifndef _M3DS_PARSER_H_
#define _M3DS_PARSER_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include "Utility/Color.h"
#include "Geometry/Compound/Compound.h"

using namespace std;

class Hash;
class Mesh;
class Material;

struct MaterialProps {
   string name;
   Color* ambient;
   Color* diffuse;
   Color* specular;
   float specHighlight;
   float highlightPercent;
   string texMap;

   MaterialProps();
};

class M3DSParser {

public:
   M3DSParser();
   ~M3DSParser() {}
   
   bool load(const string& filename);
   void setHash(Hash* h);
   GeometryObject* getMesh() { return meshs; }

private:
   void processTopLevelChunk(int nBytes);
   void processSceneChunk(int nBytes);
   void processModelChunk(int nBytes, string name);
   void processTriMeshChunk(int nBytes, string name);
   void processMaterialChunk(int nBytes);
   Color* processColorChunk(int nBytes);
   void processPercentageChunk(int nBytes, float& percent);
   void processFaceArrayChunk(int nBytes, Mesh* mesh);
   string processTexmapChunk(int nBytes);
   
   void readPointArray(Mesh* mesh);
   void readFaceArray(Mesh* mesh, int contentSize);
   void readColor(Color* color);
   void readFloatColor(Color* color);
   
   void skipBytes(int count);

   double scale;
   string textureDir;
   ifstream in;
   Compound* meshs;
   map<string, Material*> materials;
};

#endif
