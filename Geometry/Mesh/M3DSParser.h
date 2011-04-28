#ifndef _M3DS_PARSER_H_
#define _M3DS_PARSER_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Utility/Color.h"

using namespace std;

class Mesh;

struct Material {
   string name;
   int texID;
   Color ambient;
   Color diffuse;
   Color specular;
   Color emission;
   float shininess;
   float opacity;
};

class M3DSParser {

public:
   M3DSParser() {}
   ~M3DSParser() {}
   
   bool load(const string& filename);
   vector<Mesh*> getMeshs() { return meshs; }

private:
   void processTopLevelChunk(int nBytes);
   void processSceneChunk(int nBytes);
   void processModelChunk(int nBytes, string name);
   void processTriMeshChunk(int nBytes, string name);
   void processMaterialChunk(int nBytes, Material* material);
   void processColorChunk(int nBytes, Color& color);
   void processPercentageChunk(int nBytes, float& percent);

   void readPointArray(Mesh* mesh);
   void readFaceArray(Mesh* mesh, int contentSize);
   void readColor(Color& color);
   void readFloatColor(Color& color);
   
   void skipBytes(int count);

   ifstream in;
   vector<Mesh*> meshs;
};

#endif
