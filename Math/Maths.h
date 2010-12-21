#ifndef _MATHS_H_
#define _MATHS_H_

#include <math.h>

const float invRAND_MAX = 1.0 / (float) RAND_MAX;
const double INV_PI = 1.0 / M_PI;

inline float rand_float() {
	return((float)rand() * invRAND_MAX);
}

inline float rand_float(int l, float h)  {
	return (rand_float() * (h - l) + l);
}

inline int rand_int(int l, int h) {
	return ((int) (rand_float(0, h - l + 1) + l));
}

#endif
