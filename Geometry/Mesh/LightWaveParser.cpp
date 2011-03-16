#include "LightWaveParser.h"
#include "Utility/ChunkParser.h"

LightWaveParser::LightWaveParser() : Mesh(), in() {
}

void LightWaveParser::loadModel(string fname) {
   in.open(fname.c_str(), ios::in | ios::binary);
   if (!in.good()) {
      fprintf(stderr, "Lightwave Parser: Error opening %s\n", fname.c_str());
      return ;
   }
   
   string chunkID = readChunkID(in, 4);
   int size = read4ByteInt(in);
   int count = 0;

   readChunkID(in, 4); // read LWO2
   count += 4;
   
   while(count < size) {
      chunkID = readChunkID(in, 4);
      count += 4;
fprintf(stderr, "ID = %s\n", chunkID.c_str());
      if(chunkID == "TAGS") {
         count += parseTags();
      }
      else if(chunkID == "PNTS") {
         count += parsePoints();
      }
      else if(chunkID == "POLS") {
         count += parsePolygons();
      }
      else if(chunkID == "PTAG") {
         parsePTag();
      }
      else {
         count += skipChunk();
      }
   }
   
   in.close();
   
   calculateNormals();
   setupCells();
}

int LightWaveParser::parseTags() {
   int size = read4ByteInt(in);
   int count = 4;
   
   int idx = 0;

   while(count < size) {
      string tagName = readString(in);
      if(tagName.size() > 0) {
         fprintf(stderr, "%d = %s\n", idx++, tagName.c_str());
      }
      count += tagName.size() + 1;
   }
   
   return count;
}

int LightWaveParser::parsePTag() {
   int size = read4ByteInt(in);
   int count = 4;
   string subtag = readChunkID(in, 4);
   
   if(subtag == "SURF") {
      while(count < size) {
         short fidx = read2ByteInt(in);
         short tidx = read2ByteInt(in);
         printf("face %d uses %d\n", fidx, tidx);
         count += 4;
      }
   }
   else {
      char c;
      while(count < size) {
         c = readChar(in);
         count++;
      }
   }
   
   return size + 4;
}

int LightWaveParser::parsePoints() {
   int size = read4ByteInt(in);
   int count = 4;
   
   while(count < size) {
      float x = readFloat(in);
      float y = readFloat(in);
      float z = readFloat(in);
      addPoint(new Point3D(x, y, z));
      count += 12;
   }

   return count;
}

int LightWaveParser::parsePolygons() {
   int size = read4ByteInt(in);
   int count = 4;
   
   string type = readChunkID(in, 4);
   count += 4;
   int vsize;
   
   while(count < size) {
      short pointCnt = read2ByteInt(in);
      count += 2;
      
      if(pointCnt == 3) {
         int i1 = readVariableInt(in, vsize);
         count += vsize;
         
         int i2 = readVariableInt(in, vsize);
         count += vsize;

         int i3 = readVariableInt(in, vsize);
         count += vsize;

         addFace(new Face(i1, i2, i3));
      }
      else {
         printf("count = %d\n", pointCnt);
      }
   }

   return count;
}

int LightWaveParser::skipChunk() {
   int count = 0;
   int size = read4ByteInt(in);
   char c;
   
   while(count++ < size) {
      c = readChar(in);
   }
   
   return size + 4;
}
