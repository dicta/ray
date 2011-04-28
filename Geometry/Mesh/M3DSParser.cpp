#include "M3DSParser.h"
#include "Mesh.h"
#include "utility/ChunkParser.h"
#include "utility/3dschunk.h"

bool M3DSParser::load(const string& filename) {
   in.open(filename.c_str(), ios::in | ios::binary);
   if (!in.good()) {
      fprintf(stderr, "Read3DSFile: Error opening %s\n", filename.c_str());
      return false;
   }
   else {
      uint16 chunkType = readUshort(in);
      if (chunkType != M3DCHUNK_MAGIC) {
         fprintf(stderr, "Read3DSFile: Wrong magic number in header\n");
         return false;
      }

      int chunkSize = read4ByteInt(in);
      if (in.bad()) {
         fprintf(stderr, "Read3DSFile: Error reading 3DS file.\n");
         return false;
      }

      int contentSize = chunkSize - 6;
      processTopLevelChunk(contentSize);

      in.close();
//      calculateNormals();

      return true;
   }
}

void M3DSParser::processTopLevelChunk(int nBytes) {
   int bytesRead = 0;

   while (bytesRead < nBytes) {
      uint16 chunkType = readUshort(in);
      int chunkSize = read4ByteInt(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MESHDATA) {
         processSceneChunk(contentSize);
      }
      else {
         skipBytes(contentSize);
      }
   }

   if (bytesRead != nBytes) {
      cerr << "In processTopLevelChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
}

void M3DSParser::processSceneChunk(int nBytes) {
   int bytesRead = 0;

   while (bytesRead < nBytes) {
      uint16 chunkType = readUshort(in);
      int chunkSize = read4ByteInt(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_NAMED_OBJECT) {
         string name = readString(in);
         processModelChunk(contentSize - (name.length() + 1), name);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_ENTRY) {
         Material* material = new Material();
         processMaterialChunk(contentSize, material);
      }
/*      else if (chunkType == M3DCHUNK_BACKGROUND_COLOR) {
         // Don't use background color so read it in and then discard it
         Color color;
         processColorChunk(contentSize, color);
      }*/
      else {
         skipBytes(contentSize);
      }
   }

   if (bytesRead != nBytes) {
      cerr << "In processSceneChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
}

void M3DSParser::processModelChunk(int nBytes, string name) {
   int bytesRead = 0;

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshort(in);
      int chunkSize = read4ByteInt(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_TRIANGLE_MESH) {
         processTriMeshChunk(contentSize, name);
      }
      else {
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      cerr << "In processModelChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
}

void M3DSParser::processTriMeshChunk(int nBytes, string name) {
   int bytesRead = 0;
   Mesh* mesh = new Mesh();
   mesh->name = name;
   meshs.push_back(mesh);

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshort(in);
      int chunkSize = read4ByteInt(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_POINT_ARRAY) {
         readPointArray(mesh);
      }
//      else if (chunkType == M3DCHUNK_MESH_TEXTURE_COORDS) {
//         readTextureCoordArray(mesh);
//      }
      else if (chunkType == M3DCHUNK_FACE_ARRAY) {
         readFaceArray(mesh, contentSize);
      }
//      else if (chunkType == M3DCHUNK_MESH_MATRIX) {
//         readMatrix();
//      }
      else {
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      cerr << "In processTriMeshChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
   mesh->calculateNormals();
   mesh->setupCells();
}

void M3DSParser::processMaterialChunk(int nBytes, Material* material) {
   int bytesRead = 0;

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshort(in);
      int chunkSize = read4ByteInt(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MATERIAL_NAME) {
         material->name = readString(in);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_AMBIENT) {
         processColorChunk(contentSize, material->ambient);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_DIFFUSE) {
         processColorChunk(contentSize, material->diffuse);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_SPECULAR) {
         processColorChunk(contentSize, material->specular);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_SHININESS) {
         processPercentageChunk(contentSize, material->shininess);
      }
      else if (chunkType == M3DCHUNK_MATERIAL_TRANSPARENCY) {
         processPercentageChunk(contentSize, material->opacity);
      }
//      else if (chunkType == M3DCHUNK_MATERIAL_TEXMAP) {
//         processTexmapChunk(contentSize, material->texID);
//      }
      else {
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      cerr << "In processMaterialChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
}

void M3DSParser::processColorChunk(int nBytes, Color& color) {
   int bytesRead = 0;
   while (bytesRead < nBytes) {
      uint16 chunkType = readUshort(in);
      int chunkSize = read4ByteInt(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_COLOR_24) {
         readColor(color);
      }
      else if (chunkType == (M3DCHUNK_COLOR_FLOAT)) {
         readFloatColor(color);
      }
      else {
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      cerr << "In processColorChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
}

void M3DSParser::processPercentageChunk(int nBytes, float& percent) {
   int bytesRead = 0;
   while (bytesRead < nBytes) {
      uint16 chunkType = readUshort(in);
      int chunkSize = read4ByteInt(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_INT_PERCENTAGE) {
         percent = read2ByteInt(in);
      }
      else if (chunkType == M3DCHUNK_FLOAT_PERCENTAGE) {
         percent = readFloat(in);
      }
      else {
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      cerr << "In processPercentageChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
}


void M3DSParser::readPointArray(Mesh* mesh) {
   uint16 nPoints = readUshort(in);

   for (int i = 0; i < (int) nPoints; i++) {
      float x = readFloat(in);
      float y = readFloat(in);
      float z = readFloat(in);
      mesh->addPoint(new Point3D(x, y, z));
   }
}

void M3DSParser::readFaceArray(Mesh* mesh, int contentSize) {
   uint16 nFaces = readUshort(in);

   for (int i = 0; i < (int) nFaces; i++) {
      uint16 v0 = readUshort(in);
      uint16 v1 = readUshort(in);
      uint16 v2 = readUshort(in);
      uint16 flags = readUshort(in);
      mesh->addFace(new Face(v0, v1, v2));
   }

   int bytesLeft = contentSize - (8 * nFaces + 2);
   if (bytesLeft > 0) {
//      processFaceArrayChunk(bytesLeft, mesh);
      skipBytes(bytesLeft);
   }
}

void M3DSParser::readColor(Color& color) {
   unsigned char r = readChar(in);
   unsigned char g = readChar(in);
   unsigned char b = readChar(in);

   color.set((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f);
}

void M3DSParser::readFloatColor(Color& color) {
   color.set(readFloat(in) / 255.0f, readFloat(in) / 255.0f, readFloat(in) / 255.0f);
}


void M3DSParser::skipBytes(int count) {
   char c;
   while (count-- > 0) {
      in.get(c);
   }
}
