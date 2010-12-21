// tokenizer.h
//
// Copyright (C) 2001, Chris Laurel <claurel@shatters.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <string>
#include <iostream>

using namespace std;

class Tokenizer
{
public:
   enum TokenType {
      TokenName           = 0,
      TokenString         = 1,
      TokenInteger        = 2,
      TokenFloat          = 3,
      TokenBegin          = 4,
      TokenEnd            = 5,
      TokenNull           = 6,
      TokenBeginGroup     = 7,
      TokenEndGroup       = 8,
      TokenBeginArray     = 9,
      TokenEndArray       = 10,
      TokenEquals         = 11,
      TokenError          = 12,
      TokenBar            = 13,
   };

   Tokenizer(istream*);

   TokenType nextToken();
   TokenType getTokenType() const { return tokenType; }
   void pushBack() { pushedBack = true; }
   
   int getIntegerValue() const { return integerValue; }
   float getFloatValue() const { return floatValue; }
   string getNameValue() const { return textToken; }
   string getStringValue() const { return textToken; }

   int getLineNumber() const { return lineNum; }

private:
   enum State {
      StartState          = 0,
      NameState           = 1,
      NumberState         = 2,
      FractionState       = 3,
      ExponentState       = 4,
      ExponentFirstState  = 5,
      DotState            = 6,
      CommentState        = 7,
      StringState         = 8,
      ErrorState          = 9,
      StringEscapeState   = 10,
   };

   istream* in;

   int nextChar;
   TokenType tokenType;
   bool haveValidInteger;
   bool haveValidFloat;
   bool haveValidName;
   bool haveValidString;

   unsigned int unicodeValue;
   unsigned int unicodeEscapeDigits;

   bool pushedBack;

   int readChar();
   void syntaxError(const char*);

   int integerValue;
   float floatValue;
   string textToken;
   int lineNum;
};

#endif // _TOKENIZER_H_
