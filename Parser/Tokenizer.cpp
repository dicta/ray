// tokenizer.cpp
//
// Copyright (C) 2001 Chris Laurel <claurel@shatters.net>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

#include <cctype>
#include <cmath>
#include <iomanip>
#include "tokenizer.h"

static bool issep(char c) {
   return !isdigit(c) && !isalpha(c) && c != '.';
}

Tokenizer::Tokenizer(istream* _in) :
   in(_in),
   tokenType(TokenBegin),
   haveValidInteger(false),
   haveValidFloat(false),
   haveValidName(false),
   haveValidString(false),
   pushedBack(false),
   lineNum(1)
{
}

Tokenizer::TokenType Tokenizer::nextToken() {
   State state = StartState;

   if (pushedBack) {
      pushedBack = false;
      return tokenType;
   }

   textToken = "";
   haveValidInteger = false;
   haveValidFloat = false;
   haveValidName = false;
   haveValidString = false;

   if (tokenType == TokenBegin) {
      nextChar = readChar();
      if (in->eof()) {
         return TokenEnd;
      }
   }
   else if (tokenType == TokenEnd) {
      return tokenType;
   }

   integerValue = 0;
   double fractionValue = 0;
   short sign = 1;
   double fracExp = 1;
   int exponentValue = 0;
   short exponentSign = 1;

   TokenType newToken = TokenBegin;
   while (newToken == TokenBegin) {
      switch (state) {
      case StartState:
         if (isspace(nextChar)) {
            state = StartState;
         }
         else if (isdigit(nextChar)) {
            state = NumberState;
            integerValue = (int) nextChar - (int) '0';
         }
         else if (nextChar == '-') {
            state = NumberState;
            sign = -1;
            integerValue = 0;
         }
         else if (nextChar == '+') {
            state = NumberState;
            sign = +1;
            integerValue = 0;
         }
         else if (nextChar == '.') {
            state = FractionState;
            sign = +1;
            integerValue = 0;
         }
         else if (isalpha(nextChar) || nextChar == '_') {
            state = NameState;
            textToken += (char) nextChar;
         }
         else if (nextChar == '#') {
            state = CommentState;
         }
         else if (nextChar == '"') {
            state = StringState;
         }
         else if (nextChar == '{') {
            newToken = TokenBeginGroup;
            nextChar = readChar();
         }
         else if (nextChar == '}') {
            newToken = TokenEndGroup;
            nextChar = readChar();
         }
         else if (nextChar == '[') {
            newToken = TokenBeginArray;
            nextChar = readChar();
         }
         else if (nextChar == ']') {
            newToken = TokenEndArray;
            nextChar = readChar();
         }
         else if (nextChar == '=') {
            newToken = TokenEquals;
            nextChar = readChar();
         }
         else if (nextChar == '|') {
            newToken = TokenBar;
            nextChar = readChar();
         }
         else if (nextChar == -1) {
            newToken = TokenEnd;
         }
         else {
            newToken = TokenError;
            syntaxError("Bad character in stream");
         }
         break;

      case NameState:
         if (isalpha(nextChar) || isdigit(nextChar) || nextChar == '_') {
            state = NameState;
            textToken += (char) nextChar;
         }
         else {
            newToken = TokenName;
            haveValidName = true;
         }
         break;

      case CommentState:
         if (nextChar == '\n' || nextChar == '\r' || nextChar == char_traits<char>::eof())
            state = StartState;
         break;

      case StringState:
         if (nextChar == '"') {
            newToken = TokenString;
            haveValidString = true;
            nextChar = readChar();
         }
         else if (nextChar == '\\') {
            state = StringEscapeState;
         }
         else if (nextChar == char_traits<char>::eof()) {
            newToken = TokenError;
            syntaxError("Unterminated string");
         }
         else {
            state = StringState;
            textToken += (char) nextChar;
         }
         break;

      case StringEscapeState:
         if (nextChar == '\\') {
            textToken += '\\';
            state = StringState;
         }
         else if (nextChar == 'n') {
            textToken += '\n';
            state = StringState;
         }
         else if (nextChar == '"') {
            textToken += '"';
            state = StringState;
         }
         else {
            newToken = TokenError;
            syntaxError("Unknown escape code in string");
            state = StringState;
         }
         break;

      case NumberState:
         if (isdigit(nextChar)) {
            state = NumberState;
            integerValue = integerValue * 10 + (int) nextChar - (int) '0';
         }
         else if (nextChar == '.') {
            state = FractionState;
         }
         else if (nextChar == 'e' || nextChar == 'E') {
            fractionValue = integerValue;
            state = ExponentFirstState;
         }
         else if (issep(nextChar)) {
            newToken = TokenInteger;
            haveValidInteger = true;
         }
         else {
            newToken = TokenError;
            syntaxError("Bad character in number");
         }
         break;

      case FractionState:
         if (isdigit(nextChar)) {
            state = FractionState;
            fractionValue = fractionValue * 10 + nextChar - (int) '0';
            fracExp *= 10;
         }
         else if (nextChar == 'e' || nextChar == 'E') {
            state = ExponentFirstState;
         }
         else if (issep(nextChar)) {
            newToken = TokenFloat;
            haveValidFloat = true;
         } else {
            newToken = TokenError;
            syntaxError("Bad character in number");
         }
         break;

      case ExponentFirstState:
         if (isdigit(nextChar)) {
            state = ExponentState;
            exponentValue = (int) nextChar - (int) '0';
         }
         else if (nextChar == '-') {
            state = ExponentState;
            exponentSign = -1;
         }
         else if (nextChar == '+') {
            state = ExponentState;
         }
         else {
            state = ErrorState;
            syntaxError("Bad character in number");
         }
         break;

      case ExponentState:
         if (isdigit(nextChar)) {
            state = ExponentState;
            exponentValue = exponentValue * 10 + (int) nextChar - (int) '0';
         }
         else if (issep(nextChar)) {
            newToken = TokenFloat;
            haveValidFloat = true;
         }
         else {
            state = ErrorState;
            syntaxError("Bad character in number");
         }
         break;

      case DotState:
         if (isdigit(nextChar)) {
            state = FractionState;
            fractionValue = fractionValue * 10 + (int) nextChar - (int) '0';
            fracExp = 10;
         }
         else {
            state = ErrorState;
            syntaxError("'.' in stupid place");
         }
         break;

      case ErrorState:    break;  // Prevent GCC4 warnings; do nothing

      } // Switch

      if (newToken == TokenBegin) {
         nextChar = readChar();
      }
   }

   tokenType = newToken;
   if (haveValidFloat) {
      floatValue = integerValue + fractionValue / fracExp;
      if (exponentValue != 0) {
         floatValue *= pow(10.0, exponentValue * exponentSign);
      }
      floatValue *= sign;
   }
   integerValue *= sign;

   return tokenType;
}

int Tokenizer::readChar() {
   int c = (int) in->get();
   if (c == '\n')
      lineNum++;

   return c;
}

void Tokenizer::syntaxError(const char* message) {
   cerr << message << '\n';
}
