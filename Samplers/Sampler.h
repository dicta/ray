#ifndef _SAMPLER_H_
#define _SAMPLER_H_

#include <vector>
#include "Math/Point2D.h"
#include "Math/Point3D.h"

using namespace std;

class Sampler {

public:
   Sampler(const int _numSamples = 1, const int _numSets = 83);
   virtual ~Sampler();

   virtual void generateSamples() = 0;
   void shuffleXCoords();
   void shuffleYCoords();
   void setupShuffledIndices();
   void mapSamplesToUnitDisk();
   void mapSamplesToHemisphere(float exp);

   int getNumSamples() const { return numSamples; }

   static Vector3D uniformSampleCone(double u, double v, float costhetamax, const Vector3D& x, const Vector3D& y, const Vector3D& z);

   Point2D* sampleUnitSquare();
   Point2D* sampleUnitDisk();
   Point2D* sampleOneSet();
   Point3D* sampleHemisphere();

protected:
   int numSamples;
   int numSets;
   vector<Point2D*> samples;
   vector<Point2D*> diskSamples;
   vector<Point3D*> hemisphereSamples;
   vector<int> shuffledIdx;
   unsigned long count;
   int jump;
};

#endif
