#ifndef _MESH_MANAGER_
#define _MESH_MANAGER_

#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Mesh;
class Hash;
class GeometryObject;

typedef map<string, GeometryObject*>::const_iterator MeshIter;

class MeshManager {

public:
   static MeshManager& instance();
   ~MeshManager();
   
   void loadMeshes(string fname);
   GeometryObject* getMesh(string name) { return objects[name]; }
   
private:
   MeshManager();
   void loadMesh(string type, Hash* hash);
   
   static auto_ptr<MeshManager> s_instance;
   map<string, GeometryObject*> objects;
};

#endif
