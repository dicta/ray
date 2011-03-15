#ifndef _CHUNK_PARSER_H_
#define _CHUNK_PARSER_H_

#include <iostream>
#include <fstream>

using namespace std;

extern char readChar(ifstream &in);
extern string readChunkID(ifstream &in, int size);
extern int read4ByteInt(ifstream &in);
extern string readString(ifstream &in);
extern float readFloat(ifstream &in);

#endif
