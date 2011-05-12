#include "M3DSParser.h"
#include "Mesh.h"
#include "utility/ChunkParser.h"
#include "utility/3dschunk.h"
#include "Materials/Matte.h"
#include "Materials/Phong.h"

MaterialProps::MaterialProps() :
   name(""),
   ambient(NULL),
   diffuse(NULL),
   specular(NULL),
   specHighlight(1.f)
{
}

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
         printf("processTopLevelChunk %X %d\n", chunkType, chunkSize);
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
         processMaterialChunk(contentSize);
      }
      else {
         printf("processSceneChunk %X %d\n", chunkType, chunkSize);
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
         printf("processModelChunk %X %d\n", chunkType, chunkSize);
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
      else if (chunkType == M3DCHUNK_FACE_ARRAY) {
         readFaceArray(mesh, contentSize);
      }
      else {
         printf("processTriMeshChunk %X %d\n", chunkType, chunkSize);
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      cerr << "In processTriMeshChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
   mesh->calculateNormals();
   mesh->setupCells();
}

void M3DSParser::processMaterialChunk(int nBytes) {
   int bytesRead = 0;

//   Matte* material = new Matte();
   MaterialProps props;

   while(bytesRead < nBytes) {
      uint16 chunkType = readUshort(in);
      int chunkSize = read4ByteInt(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;

      if (chunkType == M3DCHUNK_MATERIAL_NAME) {
         props.name = readString(in);
//         materials[name] = material;
      }
      else if (chunkType == M3DCHUNK_MATERIAL_AMBIENT) {
         props.ambient = processColorChunk(contentSize);
//         material->setAmbientColor(processColorChunk(contentSize));
      }
      else if (chunkType == M3DCHUNK_MATERIAL_DIFFUSE) {
//         material->setDiffuseColor(processColorChunk(contentSize));
         props.diffuse = processColorChunk(contentSize);
      }
//      else if (chunkType == M3DCHUNK_MATERIAL_SPECULAR) {
//         props.specular = processColorChunk(contentSize);
//      }
      else if (chunkType == M3DCHUNK_MATERIAL_SHININESS) {
         processPercentageChunk(contentSize, props.specHighlight);
      }
//      else if (chunkType == M3DCHUNK_MATERIAL_TRANSPARENCY) {
//         float p;
//         processPercentageChunk(contentSize, p);
//      }
      else {
         printf("processMaterialChunk %X %d\n", chunkType, chunkSize);
         skipBytes(contentSize);
      }
   }
   
   if(props.specular != NULL) {
      Phong* material = new Phong();
      material->setAmbientColor(props.ambient);
      material->setDiffuseColor(props.diffuse);
      material->setSpecularColor(props.specular);
      materials[props.name] = material;
   }
   else {
      Matte* material = new Matte();
      material->setAmbientColor(props.ambient);
      material->setDiffuseColor(props.diffuse);
      materials[props.name] = material;
   }

   if(bytesRead != nBytes) {
      cerr << "In processMaterialChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
}

Color* M3DSParser::processColorChunk(int nBytes) {
   int bytesRead = 0;
   Color* color = new Color;

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
         printf("processColorChunk %X %d\n", chunkType, chunkSize);
         skipBytes(contentSize);
      }
   }

   if(bytesRead != nBytes) {
      cerr << "In processColorChunk expected " << nBytes << " bytes but read " << bytesRead << '\n';
   }
   
   return color;
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
         printf("processPercentageChunk %X %d\n", chunkType, chunkSize);
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
      readUshort(in);
      mesh->addFace(new Face(v0, v1, v2));
   }

   int bytesLeft = contentSize - (8 * nFaces + 2);
   if (bytesLeft > 0) {
      processFaceArrayChunk(bytesLeft, mesh);
   }
}

bool isPowerOfTwo(int x) {
   return (x != 0) && ((x & (x - 1)) == 0);
}

void M3DSParser::processFaceArrayChunk(int nBytes, Mesh* mesh) {
   int bytesRead = 0;
   
   while(bytesRead < nBytes) {
      uint16 chunkType = readUshort(in);
      int chunkSize = read4ByteInt(in);
      int contentSize = chunkSize - 6;
      bytesRead += chunkSize;
      
      if (chunkType == M3DCHUNK_MESH_MATERIAL_GROUP) {
         // For now, we just assume that there is only one material group
         // per triangle mesh, and that the material applies to all faces in
         // the mesh.
         string materialName = readString(in);
         uint16 nFaces = readUshort(in);
         
         for (uint16 i = 0; i < nFaces; i++) {
            readUshort(in);
         }
         
         mesh->setMaterial(materials[materialName]);
      }
      else if(chunkType == M3DCHUNK_MESH_SMOOTH_GROUP) {
         for(FaceIter it = mesh->facesBegin(), end = mesh->facesEnd(); it != end; it++) {
            unsigned int group = readUInt(in);
            for(int i = 0; i < 32; i++) {
               int mask = (int) pow(2, i);
               if(mask & group) {
                  if(mesh->smoothingGroups.find(i) == mesh->smoothingGroups.end()) {
                     mesh->smoothingGroups[i] = new SmoothingGroup();
                  }
                 (*it)->smoothGroup = group;
                 mesh->smoothingGroups[i]->addFace(*it);
               }
            }
         }
      }
      else {
         printf("processFaceArrayChunk %X %d\n", chunkType, chunkSize);
         skipBytes(contentSize);
      }
   }
}

void M3DSParser::readColor(Color* color) {
   unsigned char r = readChar(in);
   unsigned char g = readChar(in);
   unsigned char b = readChar(in);

   color->set((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f);
}

void M3DSParser::readFloatColor(Color* color) {
   color->set(readFloat(in) / 255.0f, readFloat(in) / 255.0f, readFloat(in) / 255.0f);
}


void M3DSParser::skipBytes(int count) {
   char c;
   while (count-- > 0) {
      in.get(c);
   }
}
