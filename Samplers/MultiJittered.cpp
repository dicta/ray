/*
 *  MultiJittered.cpp
 *  RayTracer
 *
 *  Created by Eric Saari on 12/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "MultiJittered.h"
#include <math.h>
#include "Math/Maths.h"

MultiJittered::MultiJittered(const int ns, const int sets) : Sampler(ns, sets) {
   generateSamples();
}

MultiJittered::~MultiJittered() {
}

void MultiJittered::generateSamples() {
   // num_samples needs to be a perfect square

	int n = (int)sqrt((float) numSamples);
	float subcell_width = 1.0 / ((float) numSamples);
	
	// fill the samples array with dummy points to allow us to use the [ ] notation when we set the 
	// initial patterns
	
	for (int j = 0; j < numSamples * numSets; j++) {
		samples.push_back(new Point2D());
   }
   
	// distribute points in the initial patterns
	
	for (int p = 0; p < numSets; p++) 
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				samples[i * n + j + p * numSamples]->x = (i * n + j) * subcell_width + rand_float(0, subcell_width);
				samples[i * n + j + p * numSamples]->y = (j * n + i) * subcell_width + rand_float(0, subcell_width);
			}
	
	// shuffle x coordinates
	
	for (int p = 0; p < numSets; p++) 
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				int k = rand_int(j, n - 1);
				float t = samples[i * n + j + p * numSamples]->x;
				samples[i * n + j + p * numSamples]->x = samples[i * n + k + p * numSamples]->x;
				samples[i * n + k + p * numSamples]->x = t;
			}
   
	// shuffle y coordinates
	
	for (int p = 0; p < numSets; p++)
		for (int i = 0; i < n; i++)		
			for (int j = 0; j < n; j++) {
				int k = rand_int(j, n - 1);
				float t = samples[j * n + i + p * numSamples]->y;
				samples[j * n + i + p * numSamples]->y = samples[k * n + i + p * numSamples]->y;
				samples[k * n + i + p * numSamples]->y = t;
         }
}
