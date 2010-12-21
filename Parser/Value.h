#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include <vector>

using namespace std;

class Hash;
class Value;

typedef vector<Value*> Array;

class Value {

public:
   enum ValueType {
      IntegerType    = 0,
      DoubleType      = 1,
      StringType     = 2,
      ArrayType      = 3,
      HashType       = 4,
   };

   Value(int i);
   Value(double f);
   Value(string s);
   Value(Array* a);
   Value(Hash* h);
   ~Value();

   ValueType getType() const;

   int getInteger() const { return data.i; }
   double getDouble() const { return data.f; }
   string getString() const { return *data.s; }
   Array* getArray() const { return data.a; }
   Hash* getHash() const;

private:
   ValueType type;

   union {
      string* s;
      int i;
      double f;
      Array* a;
      Hash* h;
   } data;
};

#endif /*VALUE_H_*/
