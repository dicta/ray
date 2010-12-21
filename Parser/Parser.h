#ifndef _PARSER_H_
#define _PARSER_H_

#include "Hash.h"
#include "Value.h"
#include "Tokenizer.h"

class Parser {

public:
   Parser(Tokenizer*);

   Hash* readHash();
   Array* readArray();
   Value* readValue();

private:
   Tokenizer* tokenizer;
};

#endif // _PARSER_H_
