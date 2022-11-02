#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  const uint64_t N; // number of lattice sites.
  uint8_t *C;       // microscopic configuration.

} TASEP_LAT;

TASEP_LAT tasep_init(uint64_t N);
void tasep_free(TASEP_LAT *tlat);
void tasep_randomize(TASEP_LAT *tlat, uint64_t x);
void tasep_evolve(TASEP_LAT *tlat, double *q_array, double alpha, double beta,
                  uint64_t mcstep, uint32_t *r);
uint64_t tasep_stabilize(TASEP_LAT *tlat, double *q_array, double alpha,
                         double beta, uint64_t mcstep_max, uint32_t *r);
double *tasep_rho(TASEP_LAT *tlat, double *q_array, double alpha, double beta,
                  uint64_t avg_step, uint32_t *r);
