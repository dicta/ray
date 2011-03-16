#ifndef _CHUNK_PARSER_H_
#define _CHUNK_PARSER_H_

#include <iostream>
#include <fstream>

using namespace std;

extern unsigned char readChar(ifstream &in);
extern string readChunkID(ifstream &in, int size);
extern int read4ByteInt(ifstream &in);
extern short read2ByteInt(ifstream &in);
extern int readVariableInt(ifstream &in, int &vsize);
extern string readString(ifstream &in);
extern float readFloat(ifstream &in);

#endif
