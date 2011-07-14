#include "Sampler.h"
#include "Math/Maths.h"
#include <algorithm>
#include <math.h>

Sampler::Sampler(const int _numSamples, const int _numSets) : numSamples(_numSamples), numSets(_numSets), count(0), jump(0) {
   setupShuffledIndices();
}

Sampler::~Sampler() {
   for(unsigned int i = 0; i < samples.size(); i++) {
      delete samples[i];
   }
   samples.clear();

   for(unsigned int i = 0; i < diskSamples.size(); i++) {
      delete diskSamples[i];
   }
   diskSamples.clear();

   for(unsigned int i = 0; i < hemisphereSamples.size(); i++) {
      delete hemisphereSamples[i];
   }
   hemisphereSamples.clear();
}

void Sampler::shuffleXCoords() {
   for(int p = 0; p < numSets; p++) {
      for(int i = 0; i < numSamples; i++) {
         int target = rand() % numSamples + p * numSamples;
         float temp = samples[i + p * numSamples + 1]->x;
         samples[i + p * numSamples + 1]->x = samples[target]->x;
         samples[target]->x = temp;
      }
   }
}

void Sampler::shuffleYCoords() {
   for(int p = 0; p < numSets; p++) {
      for(int i = 0; i < numSamples; i++) {
         int target = rand() % numSamples + p * numSamples;
         float temp = samples[i + p * numSamples + 1]->y;
         samples[i + p * numSamples + 1]->x = samples[target]->y;
         samples[target]->y = temp;
      }
   }
}

void Sampler::setupShuffledIndices() {
   shuffledIdx.reserve(numSamples * numSets);
	vector<int> indices;

	for (int j = 0; j < numSamples; j++) {
		indices.push_back(j);
   }

	for (int p = 0; p < numSets; p++) {
		random_shuffle(indices.begin(), indices.end());

		for (int j = 0; j < numSamples; j++) {
			shuffledIdx.push_back(indices[j]);
      }
	}
}

Point2D* Sampler::sampleUnitSquare() {
	if (count % numSamples == 0) {
		jump = (rand() % numSets) * numSamples;
   }
	return (samples[jump + shuffledIdx[jump + count++ % numSamples]]);
}

Point2D* Sampler::sampleUnitDisk() {
   if (count % numSamples == 0) {
		jump = (rand() % numSets) * numSamples;
   }
	return (diskSamples[jump + shuffledIdx[jump + count++ % numSamples]]);
}

Point2D* Sampler::sampleOneSet() {
   return samples[count++ % numSamples];
}

Point3D* Sampler::sampleHemisphere() {
   if (count % numSamples == 0) {
		jump = (rand() % numSets) * numSamples;
   }
	return (hemisphereSamples[jump + shuffledIdx[jump + count++ % numSamples]]);
}

void Sampler::mapSamplesToUnitDisk() {
	int size = samples.size();
	float r, phi;		// polar coordinates
	Point2D sp; 		// sample point on unit disk

	for (int j = 0; j < size; j++) {
      // map sample point to [-1, 1] X [-1,1]
		sp.x = 2.0 * samples[j]->x - 1.0;
		sp.y = 2.0 * samples[j]->y - 1.0;

		if (sp.x > -sp.y) {			// sectors 1 and 2
			if (sp.x > sp.y) {		// sector 1
				r = sp.x;
				phi = sp.y / sp.x;
			}
			else {					// sector 2
				r = sp.y;
				phi = 2.0 - sp.x / sp.y;
			}
		}
		else {						// sectors 3 and 4
			if (sp.x < sp.y) {		// sector 3
				r = -sp.x;
				phi = 4.0 + sp.y / sp.x;
			}
			else {					// sector 4
				r = -sp.y;
				if (sp.y != 0.0)	// avoid division by zero at origin
					phi = 6.0 - sp.x / sp.y;
				else
					phi  = 0.0;
			}
		}

		phi *= M_PI / 4.0;

      Point2D* p = new Point2D(r * cos(phi), r * sin(phi));
      diskSamples.push_back(p);
	}

//	samples.erase(samples.begin(), samples.end());
}

void Sampler::mapSamplesToHemisphere(float exp) {
   int size = samples.size();
	hemisphereSamples.reserve(numSamples * numSets);

	for (int j = 0; j < size; j++) {
		float cos_phi = cos(2.0 * M_PI * samples[j]->x);
		float sin_phi = sin(2.0 * M_PI * samples[j]->x);
		float cos_theta = pow((1.0 - samples[j]->y), 1.0 / (exp + 1.0));
		float sin_theta = sqrt (1.0 - cos_theta * cos_theta);
		float pu = sin_theta * cos_phi;
		float pv = sin_theta * sin_phi;
		float pw = cos_theta;
		hemisphereSamples.push_back(new Point3D(pu, pv, pw));
	}
}

Vector3D Sampler::uniformSampleCone(double u, double v, float costhetamax, const Vector3D& x, const Vector3D& y, const Vector3D& z) {
   double costheta = lerp<double>(u, costhetamax, 1.0);
   double sintheta = sqrt(1.0 - costheta * costheta);
   double phi = v * 2.0 * M_PI;
   return x * cos(phi) * sintheta + y * sinf(phi) * sintheta + z * costheta;
}
