/*
 *  Color.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Color.h"
#include <algorithm>

using namespace std;

const Color BLACK;
const Color RED(1, 0, 0);

Color::Color(float _r, float _g, float _b, float _a) {
   red = _r;
   green = _g;
   blue = _b;
   alpha = _a;
}

Color::Color(const Color& c) {
   red = c.red;
   green = c.green;
   blue = c.blue;
   alpha = c.alpha;
}

Color::Color(Array* a) {
   red = a->at(0)->getDouble();
   green = a->at(1)->getDouble();
   blue = a->at(2)->getDouble();
   alpha = 1.0;
}

void Color::set(float r, float g, float b) {
   red = r;
   green = g;
   blue = b;
}

void Color::set(Array* a) {
   red = a->at(0)->getDouble();
   green = a->at(1)->getDouble();
   blue = a->at(2)->getDouble();
   alpha = 1.0;
}

Color& Color::operator= (const Color& p) {
   red = p.red;
   green = p.green;
   blue = p.blue;
   alpha = p.alpha;
   return *this;
}

Color& Color::operator+= (const Color& c) {
   red += c.red;
   green += c.green;
   blue += c.blue;
   return *this;
}

Color& Color::operator/= (const float a) {
   red /= a;
   green /= a;
   blue /= a;
   return *this;
}

Color Color::operator*(const float a) const {
   Color result(red * a, green * a, blue * a, alpha);
   return result;
}

Color Color::operator*(const Color& c) const {
   Color result(red * c.red, green * c.green, blue * c.blue, alpha);
   return result;
}

Color Color::operator/(const float a) const {
   Color result(red / a, green / a, blue / a, alpha);
   return result;
}

Color Color::operator+(const Color& c) const {
   Color result(red + c.red, green + c.green, blue + c.blue, alpha);
   return result;
}

void Color::normalize() {
   float maxVal = max(red, max(green, blue));
   if(maxVal > 1.0) {
      red /= maxVal;
      green /= maxVal;
      blue /= maxVal;
   }
//   red = min(red, 1.0f);
//   green = min(green, 1.0f);
//   blue = min(blue, 1.0f);
}
