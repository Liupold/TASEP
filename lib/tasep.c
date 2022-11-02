#include "tasep.h"

TASEP_LAT tasep_init(uint64_t N) {
  uint8_t *C = (uint8_t *)calloc(N, sizeof(uint8_t));
  TASEP_LAT tlat = {N, C};
  return tlat;
}

void tasep_free(TASEP_LAT *tlat) {
  free(tlat->C);
  tlat->C = NULL;
}

void tasep_randomize(TASEP_LAT *tlat, uint64_t x) {
  // xor shift 64
  for (uint64_t i = 0; i < tlat->N; i++) {
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    tlat->C[i] = (x & 1);
  }
}

void tasep_evolve(TASEP_LAT *tlat, double *q_array, double alpha, double beta,
                  uint64_t mcstep, uint32_t *r) {

  double urand;
  uint64_t last = tlat->N - 1, i;

  for (uint64_t step = 0; step < mcstep; step++) {
    for (uint64_t k = 0; k < tlat->N; k++) {
      i = (uint64_t)rand_r(r) % tlat->N; // choose a random site.
      urand = (double)rand_r(r) / (double)RAND_MAX;
      // entry
      if ((i == 0) && (tlat->C[0] == 0) && (urand < alpha)) {
        tlat->C[0] = 1;
      }
      // exit
      if ((i == last) && (tlat->C[last] == 1) && (urand < beta)) {
        tlat->C[last] = 0;
        continue;
      }
      // hop
      if ((tlat->C[i] == 1) && (tlat->C[i + 1] == 0) && (urand < q_array[i])) {
        tlat->C[i] = 0;
        tlat->C[i + 1] = 1;
      }
    }
  }
}

uint64_t tasep_stabilize(TASEP_LAT *tlat, double *q_array, double alpha,
                         double beta, uint64_t mcstep_max, uint32_t *r) {

  uint64_t sn, sum, p_sum = 0;
  uint8_t *C_prev = (uint8_t *)calloc(tlat->N, sizeof(uint8_t));

  for (uint64_t k = 0; k < tlat->N; k++) {
    C_prev[k] = tlat->C[k];
  }

  for (uint64_t step = 0; step < mcstep_max; step += 100) {
    tasep_evolve(tlat, q_array, alpha, beta, 100, r);
    sum = 0;
    for (uint64_t k = 0; k < tlat->N; k++) {
      sn = (tlat->C[k] - C_prev[k]);
      sn *= sn;
      sum += sn;
      C_prev[k] = tlat->C[k];
    }
    if ((sum == p_sum) && (step > 100000)) {
      return step;
    }
    p_sum = sum;
  }
  free(C_prev);
  return mcstep_max;
}

double *tasep_rho(TASEP_LAT *tlat, double *q_array, double alpha, double beta,
                  uint64_t avg_step, uint32_t *r) {

  double *rho = (double *)calloc(tlat->N, sizeof(double));

  for (uint64_t step = 0; step < avg_step; step++) {
    tasep_evolve(tlat, q_array, alpha, beta, 1, r);
    for (uint64_t k = 0; k < tlat->N; k++) {
      rho[k] += tlat->C[k];
    }
  }

  for (uint64_t k = 0; k < tlat->N; k++) {
    rho[k] = rho[k] / (double)avg_step;
  }

  return rho;
}
