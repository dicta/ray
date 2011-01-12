#ifndef _PLY_PARSER_H_
#define _PLY_PARSER_H_

#include "Utility/ply.h"
#include "Mesh.h"

class PlyParser : public Mesh {
   
public:
   PlyParser();
   void loadModel(string file);
};

#endif
