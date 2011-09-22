#ifndef _MATHS_H_
#define _MATHS_H_

#include <math.h>
#include <cstdlib>
#include "Vector3D.h"

const float invRAND_MAX = 1.0 / (float) RAND_MAX;
const double INV_PI = 1.0 / M_PI;
const double HUGE_VALUE = 1.0E10;

/** Multiply to convert from degrees to radians */
const double DEG_TO_RAD = M_PI / 180.0;
/** Multiply to convert from radians to degrees */
const double RAD_TO_DEG = 180.0 / M_PI;

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

float normalize(const float f);

template<class T>
inline T lerp(const float f, const T a, const T b) {
   return a + (b - a) * f;
}

inline float Log2(float v) {
   static float invlog = 1.f / logf(2.f);
   return logf(v) * invlog;
}

inline int Log2Int(float v) {
   return (int) floor(Log2(v));
}

/**
 * Returns an interpolated value in range [0, 1] based on value of x relative to a and b.
 *
 * @param a Lower bound
 * @param b Upper bound
 * @param x Value to interpolate
 */
double smoothStep(double a, double b, double x);

/** Modulus for doubles. */
double mod(double a, double b);

inline double smoothPulse(double e0, double e1, double e2, double e3, double x) {
   return smoothStep(e0, e1, x) - smoothStep(e2, e3, x);
}

inline double smoothPulseTrain(double e0, double e1, double e2, double e3, double period, double x) {
   return smoothPulse(e0, e1, e2, e3, mod(x, period));
}

template<class T>
T mix(const T& a, const T& b, const double f) {
   return a * (1.0 - f) + b * f;
}

int SolveQuartic(double c[5], double s[4]);

void coordinateSystem(const Vector3D& v1, Vector3D* v2, Vector3D* v3);

#endif
