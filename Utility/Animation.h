#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <vector>
#include <string>

using namespace std;

class Hash;
class Instance;

class FrameObject {

public:
   FrameObject(Instance* i);
   void setup();

   Instance* instance;
   double rx, ry, rz;
};

struct Frame {
   void addFrameObject(FrameObject* fo) { objects.push_back(fo); }
   vector<FrameObject*> objects;
};

class Animation {

public:
   Animation();
   ~Animation();

   void setup(const string& fname);
   void play();

private:
   void loadConfiguration(Hash* hash);

   Frame* frames;
   int frameCount;
};

#endif
