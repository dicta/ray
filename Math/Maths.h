#ifndef _MATHS_H_
#define _MATHS_H_

#include <math.h>

const float invRAND_MAX = 1.0 / (float) RAND_MAX;
const double INV_PI = 1.0 / M_PI;

// Multiply to convert from degrees to radians
const double DEG_TO_RAD = M_PI / 180.0;

inline float rand_float() {
	return((float)rand() * invRAND_MAX);
}

inline float rand_float(int l, float h)  {
	return (rand_float() * (h - l) + l);
}

inline int rand_int(int l, int h) {
	return ((int) (rand_float(0, h - l + 1) + l));
}

inline float clamp(const float x, const float min, const float max) {
   return x < min ? min : x > max ? max : x;
}

inline float lerp(const float f, const float a, const float b) {
   return a + f * (b - a);
}

int SolveQuartic(double c[5], double s[4]);

#endif
