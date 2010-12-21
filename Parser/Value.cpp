#include "Value.h"
#include "Hash.h"

Value::Value(int i) {
   type = IntegerType;
   data.i = i;
}

Value::Value(double f) {
   type = DoubleType;
   data.f = f;
}

Value::Value(string s) {
   type = StringType;
   data.s = new string(s);
}

Value::Value(Array* a) {
   type = ArrayType;
   data.a = a;
}

Value::Value(Hash* h) {
   type = HashType;
   data.h = h;
}

Value::~Value() {
   if (type == StringType) {
      delete data.s;
   }
   else if (type == HashType) {
      if (data.h != NULL) {
         delete data.h;
      }
   }
}

Hash* Value::getHash() const {
   return data.h;
}

Value::ValueType Value::getType() const {
   return type;
}
