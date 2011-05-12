#ifndef _M3DS_PARSER_H_
#define _M3DS_PARSER_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include "Utility/Color.h"

using namespace std;

class Mesh;
class Material;

struct MaterialProps {
   string name;
   Color* ambient;
   Color* diffuse;
   Color* specular;
   float specHighlight;

   MaterialProps();
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
   void processMaterialChunk(int nBytes);
   Color* processColorChunk(int nBytes);
   void processPercentageChunk(int nBytes, float& percent);
   void processFaceArrayChunk(int nBytes, Mesh* mesh);
   
   void readPointArray(Mesh* mesh);
   void readFaceArray(Mesh* mesh, int contentSize);
   void readColor(Color* color);
   void readFloatColor(Color* color);
   
   void skipBytes(int count);

   ifstream in;
   vector<Mesh*> meshs;
   map<string, Material*> materials;
};

#endif
