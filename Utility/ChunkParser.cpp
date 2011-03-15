#include "ChunkParser.h"
#include "bytes.h"

char readChar(ifstream &in) {
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

int read4ByteInt(ifstream &in) {
   int ret;
   in.read((char *) &ret, 4);
   LE_TO_CPU_INT32(ret, ret);
   return ret;
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

float readFloat(ifstream &in) {
   float f;
   in.read((char*) &f, sizeof(float));
   LE_TO_CPU_FLOAT(f, f);
   return f;
}
