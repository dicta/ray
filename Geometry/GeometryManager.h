#ifndef _GEOMETRY_MANAGER_H_
#define _GEOMETRY_MANAGER_H_

#include <map>
#include <memory>
#include <string>

using namespace std;

class GeometryObject;
class Hash;
class Storage;

typedef map<string, GeometryObject*>::const_iterator GeometryIter;

class GeometryManager {

public:
   static GeometryManager& instance();
   ~GeometryManager();

   void loadObjects(string fname);

   GeometryIter begin() const { return objects.begin(); }
   GeometryIter end() const { return objects.end(); }

   GeometryObject* createObject(string type, Hash* hash, bool addToList = true);
   GeometryObject* removeObject(string name);

   Storage* getStorage() { return storage; }

private:
   GeometryManager();

   static auto_ptr<GeometryManager> s_instance;
   map<string, GeometryObject*> objects;
   Storage* storage;
};

#endif
