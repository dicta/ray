// hash.h
//
// Copyright (C) 2009, Eric Saari
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef HASH_H_
#define HASH_H_

#include <map>
#include <string>

using namespace std;

class Value;

typedef map<string, Value *>::const_iterator HashIterator;

class Hash {
   
public:
   Hash();
   ~Hash();
   
   void addValue(string key, Value &value);
   Value *getValue(const string &key) const;

   int getInteger(const string &key) const;
   double getDouble(const string &key) const;
   string getString(const string& key) const;
   
   bool contains(const string &key) const;
   
   HashIterator begin();
   HashIterator end();
   
private:
   map<string, Value *> data;
};

#endif /*HASH_H_*/
