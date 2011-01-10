#include "LatticeNoise.h"
#include "Math/Maths.h"
#include "Parser/Hash.h"
#include "Samplers/MultiJittered.h"

/*
 * The following table is by Darwyn Peachey in Ebert et al. (2003), page 70.
 */
const unsigned char
LatticeNoise::permutations[256] =
						{
					        225,155,210,108,175,199,221,144,203,116, 70,213, 69,158, 33,252,
					        5, 82,173,133,222,139,174, 27,  9, 71, 90,246, 75,130, 91,191,
					        169,138,  2,151,194,235, 81,  7, 25,113,228,159,205,253,134,142,
					        248, 65,224,217, 22,121,229, 63, 89,103, 96,104,156, 17,201,129,
					        36,  8,165,110,237,117,231, 56,132,211,152, 20,181,111,239,218,
					        170,163, 51,172,157, 47, 80,212,176,250, 87, 49, 99,242,136,189,
					        162,115, 44, 43,124, 94,150, 16,141,247, 32, 10,198,223,255, 72,
					        53,131, 84, 57,220,197, 58, 50,208, 11,241, 28,  3,192, 62,202,
					        18,215,153, 24, 76, 41, 15,179, 39, 46, 55,  6,128,167, 23,188,
					        106, 34,187,140,164, 73,112,182,244,195,227, 13, 35, 77,196,185,
					        26,200,226,119, 31,123,168,125,249, 68,183,230,177,135,160,180,
					        12,  1,243,148,102,166, 38,238,251, 37,240,126, 64, 74,161, 40,
					        184,149,171,178,101, 66, 29, 59,146, 61,254,107, 42, 86,154,  4,
					        236,232,120, 21,233,209, 45, 98,193,114, 78, 19,206, 14,118,127,
					        48, 79,147, 85, 30,207,219, 54, 88,234,190,122, 95, 67,143,109,
					        137,214,145, 93, 92,100,245,  0,216,186, 60, 83,105, 97,204, 52
				    	};

LatticeNoise::LatticeNoise(int seed) : numOctaves(1), lacunarity(1.0), gain(1.0), fsMin(0.0), fsMax(1.0) {
   initValueTable(seed);
   initVectorTable(seed);
}

LatticeNoise::~LatticeNoise() {
}

void LatticeNoise::initValueTable(int seed) {
   srand(seed);
   for(int i = 0; i < 256; i++) {
      // clamp range to [-1.0, 1.0]
      values[i] = 1.0 - 2.0 * rand_float();
   }
}

void LatticeNoise::initVectorTable(int seed) {
   srand(seed);
   MultiJittered sampler(256, 1);
   
   for(int i = 0; i < 256; i++) {
      Point2D* point = sampler.sampleOneSet();
      float z = 1.0 - 2.0 * point->x;
      float r = sqrt(1.0 - z * z);
      float phi = 2.0 * M_PI * point->y;
      float x = r * cos(phi);
      float y = r * sin(phi);
      vectors[i].set(x, y, z);
      vectors[i].normalize();
   }
}

void LatticeNoise::setHash(Hash* h) {
   numOctaves = h->getInteger("numOctaves");
   
   if(h->contains("lacunarity")) {
      lacunarity = h->getDouble("lacunarity");
   }
   else {
      lacunarity = 1.0;
   }
   if(h->contains("gain")) {
      gain = h->getDouble("gain");
   }
   else {
      gain = 1.0;
   }

   // Compute the bounds
   if(gain == 1.0) {
      fsMax = numOctaves;
   }
   else {
      fsMax = (1.0 - pow(gain, numOctaves)) / (1.0 - gain);
   }
   fsMin = -fsMax;
}

float LatticeNoise::fractalSum(const Point3D& p) const {
   float amplitude = 1.0;
   float frequency = 1.0;
   float sum = 0.0;
   
   for(int j = 0; j < numOctaves; j++) {
      sum += amplitude * valueNoise(p * frequency);
      amplitude *= 0.5;
      frequency *= 2.0;
   }
   
   // Map to range [0, 1]
   sum = (sum - fsMin) / (fsMax - fsMin);
   
   return sum;
}

float LatticeNoise::turbulence(const Point3D& p) const {
   float amplitude = 1.0;
   float frequency = 1.0;
   float sum = 0.0;

   for(int j = 0; j < numOctaves; j++) {
      sum += amplitude * fabs(valueNoise(p * frequency));
      amplitude *= 0.5;
      frequency *= 2.0;
   }

   // Map to [0, 1]
//   sum /= fsMax;
   return sum;
}

Vector3D LatticeNoise::vectorTurbulence(const Point3D& p) const {
   float amplitude = 1.0;
   float frequency = 1.0;
   Vector3D sum;
   
   for(int j = 0; j < numOctaves; j++) {
      sum += vectorNoise(p * frequency).absValue() * amplitude;
      amplitude *= 0.5;
      frequency *= 2.0;
   }
   
   return sum;
}

float LatticeNoise::fbm(const Point3D& p) const {
   float amplitude = 1.0;
   float frequency = 1.0;
   float sum = 0.0;
   
   for(int j = 0; j < numOctaves; j++) {
      sum += amplitude * valueNoise(p * frequency);
      amplitude *= gain;
      frequency *= lacunarity;
   }

   // Map to [0, 1]
   sum = (sum - fsMin) / (fsMax - fsMin);
   return sum;
}

Vector3D LatticeNoise::vectorFbm(const Point3D& p) const {
   float amplitude = 1.0;
   float frequency = 1.0;
   Vector3D sum;
   
   for(int j = 0; j < numOctaves; j++) {
      sum += vectorNoise(p * frequency) * amplitude;
      amplitude *= gain;
      frequency *= lacunarity;
   }
   
   return sum;
}
