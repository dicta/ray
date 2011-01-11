#ifndef _MEDIT_PARSER_H_
#define _MEDIT_PARSER_H_

#include "Mesh.h"

using namespace std;

class MeditParser : public Mesh {

public:
   MeditParser();
   void readMesh(string fname);
   
private:
   void readTriangles(int count, ifstream& fp);
};

#endif
