#ifndef _CHUNK_PARSER_H_
#define _CHUNK_PARSER_H_

#include <iostream>
#include <fstream>

using namespace std;

typedef unsigned short uint16;

extern unsigned char readChar(ifstream &in);
extern string readChunkID(ifstream &in, int size);

extern int readIntLE(ifstream &in);
extern int readIntBE(ifstream &in);
extern unsigned int readUIntLE(ifstream& in);
extern unsigned int readUIntBE(ifstream& in);

extern short readShortLE(ifstream &in);
extern short readShortBE(ifstream &in);
extern uint16 readUshortLE(ifstream& in);
extern uint16 readUshortBE(ifstream& in);

extern int readVariableIntBE(ifstream &in, int &vsize);
extern string readString(ifstream &in);

extern float readFloatLE(ifstream &in);
extern float readFloatBE(ifstream &in);

#endif
