#ifndef _CHUNK_PARSER_H_
#define _CHUNK_PARSER_H_

#include <iostream>
#include <fstream>

using namespace std;

typedef unsigned short uint16;

extern unsigned char readChar(ifstream &in);
extern string readChunkID(ifstream &in, int size);
extern int read4ByteInt(ifstream &in);
extern unsigned int readUInt(ifstream& in);
extern short read2ByteInt(ifstream &in);
extern uint16 readUshort(ifstream& in);
extern int readVariableInt(ifstream &in, int &vsize);
extern string readString(ifstream &in);
extern float readFloat(ifstream &in);

#endif
