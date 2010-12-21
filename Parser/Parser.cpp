#include "Parser.h"

Parser::Parser(Tokenizer* _tokenizer) : tokenizer(_tokenizer) {
}

Array* Parser::readArray() {
   Tokenizer::TokenType tok = tokenizer->nextToken();
   if (tok != Tokenizer::TokenBeginArray) {
      tokenizer->pushBack();
      return NULL;
   }
   
   Array* array = new Array();
   
   Value* v = readValue();
   while (v != NULL) {
      array->insert(array->end(), v);
      v = readValue();
   }
   
   tok = tokenizer->nextToken();
   if (tok != Tokenizer::TokenEndArray) {
      tokenizer->pushBack();
      delete array;
      return NULL;
   }
   
   return array;
}

Hash* Parser::readHash() {
   Tokenizer::TokenType tok = tokenizer->nextToken();
   if (tok != Tokenizer::TokenBeginGroup) {
      tokenizer->pushBack();
      return NULL;
   }

   Hash* hash = new Hash();

   tok = tokenizer->nextToken();
   while (tok != Tokenizer::TokenEndGroup) {
      if (tok != Tokenizer::TokenName) {
         tokenizer->pushBack();
         delete hash;
         return NULL;
      }
      string name = tokenizer->getNameValue();

      Value* value = readValue();
      if (value == NULL) {
         delete hash;
         return NULL;
      }
        
      hash->addValue(name, *value);
      tok = tokenizer->nextToken();
   }

   return hash;
}

Value* Parser::readValue() {
   Tokenizer::TokenType tok = tokenizer->nextToken();
   switch (tok) {
      case Tokenizer::TokenInteger:
         return new Value(tokenizer->getIntegerValue());

      case Tokenizer::TokenFloat:
         return new Value(tokenizer->getFloatValue());

      case Tokenizer::TokenString:
         return new Value(tokenizer->getStringValue());

      case Tokenizer::TokenBeginArray:
         tokenizer->pushBack();
         {
            Array* array = readArray();
            if (array == NULL) {
               return NULL;
            }
            else {
               return new Value(array);
            }
         }

      case Tokenizer::TokenBeginGroup:
         tokenizer->pushBack();
         {
            Hash* hash = readHash();
            if (hash == NULL) {
               return NULL;
            }
            else {
               return new Value(hash);
            }
         }
      
      default:
         tokenizer->pushBack();
         return NULL;
   }
}
