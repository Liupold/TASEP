#include "tasep.h"

double *rho_gen_data(uint64_t N, double alpha, double beta, double *q,
                     uint32_t r) {
  double *rho;
  uint64_t step;

  TASEP_LAT tlat = tasep_init(N);
  tasep_randomize(&tlat, (uint64_t)r);

  step = tasep_stabilize(&tlat, q, alpha, beta, 10000000000, &r);
  rho = tasep_rho(&tlat, q, alpha, beta, step, &r);

  tasep_free(&tlat);
  return rho;
}
