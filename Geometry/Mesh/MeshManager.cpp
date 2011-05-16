#include <fstream>
#include "MeshManager.h"
#include "Mesh.h"
#include "parser/Parser.h"
#include "MeditParser.h"
#include "PlyParser.h"
#include "LightWaveParser.h"
#include "M3DSParser.h"

auto_ptr<MeshManager> MeshManager::s_instance;

MeshManager& MeshManager::instance() {
   if(s_instance.get() == 0) {
      s_instance.reset(new MeshManager());
   }
   return *s_instance;
}

MeshManager::MeshManager() {
}

MeshManager::~MeshManager() {
   for(MeshIter it = objects.begin(); it != objects.end(); it++) {
//      delete (*it).second;
   }
   objects.clear();
}

void MeshManager::loadMeshes(string fname) {
   ifstream fp(fname.c_str());
   Tokenizer tok(&fp);
   Parser parser(&tok);

   while(tok.nextToken() != Tokenizer::TokenEnd) {
      if(tok.getTokenType() != Tokenizer::TokenName) {
         return ;
      }

      string type = tok.getStringValue();
      loadMesh(type, parser.readValue()->getHash());
   }

   fp.close();
}

void MeshManager::loadMesh(string type, Hash* hash) {
   string objName = hash->getString("name");
   string filename = hash->getString("filename");
   vector<Mesh*> vec;

   if(type == "medit") {
      MeditParser* mp = new MeditParser();
      mp->readMesh(filename);
      vec.push_back(mp);
   }
   else if(type == "ply") {
      PlyParser* p = new PlyParser();
      p->loadModel(filename);
      vec.push_back(p);
   }
   else if(type == "lwo") {
      LightWaveParser* p = new LightWaveParser();
      p->loadModel(filename);
//      vec.push_back(p);
   }
   else if(type == "m3ds") {
      M3DSParser parser;
      parser.setHash(hash);
      parser.load(filename);
      vec = parser.getMeshs();
   }

   objects[objName] = vec;
}
