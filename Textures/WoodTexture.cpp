#include "WoodTexture.h"
#include "Noise/CubicNoise.h"
#include "Noise/LinearNoise.h"
#include "Math/Maths.h"

WoodTexture::WoodTexture() : Texture(), noise(NULL) {
//   lightColor.set(0.92, 0.92, 0.575);
lightColor.set(0.5, 0.2, 0.065);
   darkColor.set(0.05, 0.05, 0.05); // 0.40, 0.28, 0.08);
   ringFreq = 1.8;
   ringUneveness = 0.25;
   ringNoise = 0.02;
   ringNoiseFreq = 1.0;
   trunkWobble = 0.15;
   trunkWobbleFreq = 0.025;
   angularWobble = 0.5;
   angularWobbleFreq = 1.0;
   grainFreq = 25.0;
   grainy = 0.5;
   ringy = 0.5;
}

WoodTexture::~WoodTexture() {
   delete noise;
}

void WoodTexture::setHash(Hash* hash) {
   noise = new CubicNoise();
   noise->setHash(hash);
}

Color WoodTexture::getColor(const ShadeRecord& sr) const {
   // Perturb the hit point
   Vector3D offset = noise->vectorFbm(sr.localHitPoint * ringNoiseFreq);
   Point3D ringPoint = sr.localHitPoint + offset * ringNoise;
   
   // Perturb the trunk along z axis
   Vector3D tempV = noise->vectorNoise(Point3D(0, 0, sr.localHitPoint.y * trunkWobbleFreq)) * trunkWobble;
   ringPoint.x += tempV.x;
   ringPoint.z += tempV.z;
   
   // Distance from y axis
   double r = sqrt(pow(ringPoint.x, 2.0) + pow(ringPoint.z, 2.0)) * ringFreq;
   
   // Perturb r so rings are not perfectly round
   Point3D tempP(ringPoint * angularWobbleFreq);
   tempP.y *= 0.1;
   r += angularWobble * smoothStep(0.0, 5.0, r) * noise->valueNoise(tempP);
   
   // Add noise so rings are not equally spaced and have different thicknesses
   r += ringUneveness * noise->valueNoise(Point3D(r));
   double inRing = smoothPulseTrain(0.1, 0.55, 0.7, 0.95, 1.0, r);
   
   // Define the grain
   Point3D grainPoint(sr.localHitPoint * grainFreq);
   grainPoint.y *= 0.05;
   
   double dpGrain = 0.2;
   double grain = 0.0;
   double amplitude = 1.0;

   for(int i = 0; i < 2; i++) {
      double grainValid = 1.0 - smoothStep(0.2, 0.6, dpGrain);
      if(grainValid > 0.0) {
         double g = grainValid * noise->valueNoise(grainPoint);
         g *= (0.3 + 0.7 * inRing);
         g = pow(clamp(0.8 - g, 0.0, 1.0), 2.0);
         g = grainy * smoothStep(0.5, 1.0, g);
         if(i == 0) {
            inRing *= (1.0 - 0.4 * grainValid);
         }
         grain = amplitude * max(grain, g);
      }
      
      grainPoint *= 2.0;
      dpGrain *= 2.0;
      amplitude *= 0.5;
   }
   
   double value = mix(inRing * ringy, 1.0, grain);
   return mix(lightColor, darkColor, value);
}

