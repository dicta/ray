#ifndef _MESH_MANAGER_
#define _MESH_MANAGER_

#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Mesh;
class Hash;

typedef map<string, vector<Mesh*> >::const_iterator MeshIter;
typedef vector<Mesh*>::const_iterator VecIter;

class MeshManager {

public:
   static MeshManager& instance();
   ~MeshManager();
   
   void loadMeshes(string fname);
   vector<Mesh*> getMesh(string name) { return objects[name]; }
   
private:
   MeshManager();
   void loadMesh(string type, Hash* hash);
   
   static auto_ptr<MeshManager> s_instance;
   map<string, vector<Mesh*> > objects;
};

#endif
