/*
 *  Color.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _COLOR_H_
#define _COLOR_H_

#include <SDL/SDL.h>
#include "Parser/Value.h"

class Color {
   
public:
   Color(float _r = 0, float _g = 0, float _b = 0, float _a = 1);
   Color(const Color& c);
   Color(Array* a);

   void set(float r, float b, float g);
   void set(Array* a);

   Color& operator=(const Color& c);
   Color& operator+=(const Color& c);
   Color& operator+=(const float a);
   Color& operator/= (const float a);
   Color operator*(const float a) const;
   Color operator*(const Color& c) const;
   Color operator/(const float a) const;
   Color operator+(const Color& c) const;
   
   void normalize();
   
   inline Uint8 getRed() const { return (Uint8)(red * 255); }
   inline Uint8 getGreen() const { return (Uint8)(green * 255); }
   inline Uint8 getBlue() const { return (Uint8)(blue * 255); }
   
   float red, green, blue, alpha;
};

inline Color Color::operator*(const float a) const {
   return Color(red * a, green * a, blue * a, alpha);
}

inline Color Color::operator*(const Color& c) const {
   return Color(red * c.red, green * c.green, blue * c.blue, alpha);
}

inline Color Color::operator/(const float a) const {
   return Color(red / a, green / a, blue / a, alpha);
}

inline Color Color::operator+(const Color& c) const {
   return Color(red + c.red, green + c.green, blue + c.blue, alpha);
}

extern const Color BLACK;
extern const Color RED;

#endif
