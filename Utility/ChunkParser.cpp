#include "ChunkParser.h"
#include "bytes.h"

unsigned char readChar(ifstream &in) {
   char c;
   in.read(&c, 1);
   return c;
}

string readChunkID(ifstream &in, int size) {
   string s;
   for(int i = 0; i < size; i++) {
      s += readChar(in);
   }
   return s;
}

int readIntLE(ifstream &in) {
   int ret;
   in.read((char *) &ret, 4);
   LE_TO_CPU_INT32(ret, ret);
   return ret;
}

int readIntBE(ifstream &in) {
   int ret;
   in.read((char *) &ret, 4);
   BE_TO_CPU_INT32(ret, ret);
   return ret;
}

unsigned int readUIntLE(ifstream &in) {
   unsigned int ret;
   in.read((char *) &ret, 4);
   LE_TO_CPU_INT32(ret, ret);
   return ret;
}

unsigned int readUIntBE(ifstream &in) {
   unsigned int ret;
   in.read((char *) &ret, 4);
   BE_TO_CPU_INT32(ret, ret);
   return ret;
}

short readShortLE(ifstream &in) {
   short ret;
   in.read((char *) &ret, 2);
   LE_TO_CPU_INT16(ret, ret);
   return ret;
}

short readShortBE(ifstream &in) {
   short ret;
   in.read((char *) &ret, 2);
   BE_TO_CPU_INT16(ret, ret);
   return ret;
}

uint16 readUshortLE(ifstream &in) {
   uint16 ret;
   in.read((char *) &ret, sizeof(uint16));
   LE_TO_CPU_INT16(ret, ret);
   return ret;
}

uint16 readUshortBE(ifstream &in) {
   uint16 ret;
   in.read((char *) &ret, sizeof(uint16));
   BE_TO_CPU_INT16(ret, ret);
   return ret;
}

int readVariableIntBE(ifstream &in, int & vsize) {
   streampos pos = in.tellg();
   unsigned char c = readChar(in);
   in.seekg(pos);
   
   if(c == 0xFF) {
      vsize = 4;
      return readIntBE(in);
   }
   
   vsize = 2;
   return readShortBE(in);
}

string readString(ifstream &in) {
   char s[1024];

   for (int count = 0; count < 1024; count++) {
      in.read(s + count, 1);
      if (s[count] == '\0') {
         break;
      }
   }

   return string(s);
}

float readFloatLE(ifstream &in) {
   float f;
   in.read((char*) &f, sizeof(float));
   LE_TO_CPU_FLOAT(f, f);
   return f;
}

float readFloatBE(ifstream &in) {
   float f;
   in.read((char*) &f, sizeof(float));
   BE_TO_CPU_FLOAT(f, f);
   return f;
}
