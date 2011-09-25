#include "LightWaveParser.h"
#include "Utility/ChunkParser.h"
#include "Materials/Matte.h"

LightWaveParser::LightWaveParser() : Mesh(), in() {
}

void LightWaveParser::loadModel(string fname) {
   in.open(fname.c_str(), ios::in | ios::binary);
   if (!in.good()) {
      fprintf(stderr, "Lightwave Parser: Error opening %s\n", fname.c_str());
      return ;
   }

   string chunkID = readChunkID(in, 4);
   int size = readIntBE(in);
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
      else if(chunkID == "SURF") {
         parseSurface();
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
   int size = readIntBE(in);
   int count = 4;

   int idx = 0;

   while(count < size) {
      string tagName = readString(in);
      if(tagName.size() > 0) {
         fprintf(stderr, "%d = %s\n", idx++, tagName.c_str());
         materialMap[tagName] = new Matte;
         tags.push_back(tagName);
      }
      count += tagName.size() + 1;
   }

   return count;
}

int LightWaveParser::parsePTag() {
   int size = readIntBE(in);
   int count = 4;
   string subtag = readChunkID(in, 4);

   if(subtag == "SURF") {
      while(count < size) {
         short fidx = readShortBE(in);
         short tidx = readShortBE(in);
         printf("face %d uses %d\n", fidx, tidx);
//         faces[fidx]->materialName = tags[tidx];
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
   int size = readIntBE(in);
   int count = 4;

   while(count < size) {
      float x = readFloatBE(in);
      float y = readFloatBE(in);
      float z = readFloatBE(in);
      addPoint(new Point3D(x, y, z));
      count += 12;
   }

   return count;
}

int LightWaveParser::parsePolygons() {
   int size = readIntBE(in);
   int count = 4;

   string type = readChunkID(in, 4);
   count += 4;
   int vsize;

   while(count < size) {
      short pointCnt = readShortBE(in);
      count += 2;

      if(pointCnt == 3) {
         int i1 = readVariableIntBE(in, vsize);
         count += vsize;

         int i2 = readVariableIntBE(in, vsize);
         count += vsize;

         int i3 = readVariableIntBE(in, vsize);
         count += vsize;

         addFace(i1, i2, i3);
      }
      else {
         printf("count = %d\n", pointCnt);
      }
   }

   return count;
}

int LightWaveParser::parseSurface() {
   int size = readIntBE(in);
   int count = 4;

   string sname = readString(in);
   count += sname.size() + 1;
   if(sname.size() % 2 == 0) {
      count++;
      readChar(in);
   }

   string pname = readString(in);
   count += pname.size() + 1;
   if(pname.size() % 2 == 0) {
      count++;
      readChar(in);
   }

   while(count < size) {
      string subName = readChunkID(in, 4);
      short subSize = readShortBE(in);
      count += 6;

      if(subName == "COLR") {
         float r = readFloatBE(in);
         float b = readFloatBE(in);
         float g = readFloatBE(in);
         readShortBE(in);
         printf("Color = %f, %f, %f\n", r, g, b);
         materialMap[sname]->setColor(r, g, b);
         materialMap[sname]->setDiffuse(1.0);
      }
      else if(subName == "DIFF") {
         float value = readFloatBE(in);
         readShortBE(in);
         printf("DIFF = %f\n", value);
         materialMap[sname]->setDiffuse(value);
      }
      else {
         printf("subName = %s\n", subName.c_str());
         for(int i = 0; i < subSize; i++) {
            readChar(in);
         }
      }
      count += subSize;
   }

   return count;
}

int LightWaveParser::skipChunk() {
   int count = 0;
   int size = readIntBE(in);
   char c;

   while(count++ < size) {
      c = readChar(in);
   }

   return size + 4;
}

