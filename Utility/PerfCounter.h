/*
 *  PerfCounter.h
 *  RayTracer
 *
 *  Created by Eric Saari on 12/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PERFCOUNTER_H_
#define _PERFCOUNTER_H_

#include <stdint.h>


#ifndef NO_COUNTERS

#define DEFCOUNTER(counter_name)                 \
   uint64_t counter_name;                        \
   inline void increment_##counter_name() {      \
      __sync_fetch_and_add (&(counter_name), 1); \
   }

#else 

#define DEFCOUNTER(counter_name)                 \
   uint64_t counter_name;                        \
   inline void increment_##counter_name() {}     

#endif


/**
 * This structure really needs to be smaller than a cache line in order to not negatively impact performance.
 * Also, since we're using a single set of counters that will be shared across multiple threads,
 * stores to this structure must be atomic.
 */
class PerfCounter {
public:

  DEFCOUNTER(num_primary_rays)
  DEFCOUNTER(num_shadow_rays)

  DEFCOUNTER(primary_nodes_traversed)
  DEFCOUNTER(primary_hits)
  DEFCOUNTER(primary_misses)

  DEFCOUNTER(shadow_nodes_traversed)
  DEFCOUNTER(shadow_hits)
  DEFCOUNTER(shadow_misses)
};

extern class PerfCounter performance_counter;

#endif
