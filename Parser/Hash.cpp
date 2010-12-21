// hash.cpp
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include "Hash.h"
#include "Value.h"

Hash::Hash() {
}

Hash::~Hash() {
   for (HashIterator iter = data.begin(); iter != data.end(); iter++) {
      delete iter->second;
   }
   data.clear();
}

void Hash::addValue(string key, Value &value) {
   data.insert(map<string, Value*>::value_type(key, &value));
}

Value* Hash::getValue(const string &key) const {
   map<string, Value*>::const_iterator iter = data.find(key);
   if (iter == data.end()) {
      return NULL;
   }
   else {
      return iter->second;
   }
}

int Hash::getInteger(const string &key) const {
   Value *v = getValue(key);
   if(v == NULL || v->getType() != Value::IntegerType) {
      return 0;
   }
   return v->getInteger();
}

double Hash::getDouble(const string &key) const {
   Value *v = getValue(key);
   if(v == NULL || v->getType() != Value::DoubleType) {
      return 0;
   }
   return v->getDouble();
}

string Hash::getString(const string& key) const {
   Value *v = getValue(key);
   if(v == NULL || v->getType() != Value::StringType) {
      return "";
   }
   return v->getString();   
}

bool Hash::contains(const string &key) const {
   HashIterator it = data.find(key);
   return it != data.end();
}

HashIterator Hash::begin() {
   return data.begin();
}

HashIterator Hash::end() {
   return data.end();
}
