#include "tasep.h"
#include "q_func.h"
#include <stdio.h>

double rho_bar(uint64_t N, double alpha,
                double beta, double *q, uint32_t r) {
  double *rho, rho_avg=0;
  uint64_t step;

  TASEP_LAT tlat = tasep_init(N);
  tasep_randomize(&tlat, (uint64_t)r);
  step = tasep_stabilize(&tlat, q, alpha, beta, 10000000000, &r);
  rho = tasep_rho(&tlat, q, alpha, beta, step, &r);
  tasep_free(&tlat);
  for (uint64_t k = 1; k < N-1; k++) {
          rho_avg += rho[k];
  }
  return rho_avg / (double) N;
}

int main(void) {
  uint64_t N = 100;
  double *q, x, dx, alpha, beta, rho_avg, d_param;
  double (*qf)(double);
  uint32_t r = 42897492;

  qf = q1;
  alpha = 0.05;
  beta = 0.07;

  q = (double *)calloc(N, sizeof(double));
  dx = 1.0 / (double)(N - 1);
  x = 0;
  for (uint64_t k = 0; k < N; k++) {
    q[k] = qf(x);
    x += dx;
  }

  d_param = 0.1 / (200 - 1);
#pragma omp parallel for
  for (uint32_t j = 0; j < 200; j++) {
    rho_avg = rho_bar(N, alpha, beta, q, r);
    printf("%f, %f\n", alpha, rho_avg);
    alpha += d_param;
  }

}
