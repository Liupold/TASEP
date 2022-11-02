#include "rho_gen_data.h"
#include <stdio.h>

double *q_choice_1(TASEP_LAT *tlat) {
  double *q = (double *)calloc(tlat->N, sizeof(double));
  double dx = 1.0 / (double)(tlat->N - 1), x = 0;
  for (uint64_t k = 0; k < tlat->N; k++) {
    if (x <= 0.7) {
      q[k] = 0.5 * (2 - x * x / 0.49);
    } else {
      q[k] = 0.5 * (2 - (x - 1.4) * (x - 1.4) / 0.49);
    }
    x += dx;
  }
  return q;
}

int main(void) {
  uint64_t N = 1000;
  uint32_t r = 138901483, seed_avg = 100;
  double *rho_sum = (double *)calloc(N, sizeof(double));
  double *rho;

  double alpha = 0.1, beta = 0.7;

#pragma omp parallel for
  for (uint32_t i = 0; i < seed_avg; i++) {
    rho = rho_gen_data(N, alpha, beta, q_choice_1, r + 248420 * i);
    for (uint64_t k = 0; k < N; k++) {
      rho_sum[k] += rho[k];
    }
    fprintf(stderr, "%u done!\n", i);
    free(rho);
  }
  for (uint64_t k = 0; k < N; k++) {
    printf("%f\n", rho_sum[k] / seed_avg);
  }
  free(rho_sum);
}
