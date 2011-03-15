#ifndef _LIGHTWAVE_PARSER_H_
#define _LIGHTWAVE_PARSER_H_

#include "Mesh.h"

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class LightWaveParser : public Mesh {

public:
   LightWaveParser();
   
   void loadModel(string fname);
   
private:
   ifstream in;
   
   int parseTags();
   int parsePoints();
   int skipChunk();
};

#endif
