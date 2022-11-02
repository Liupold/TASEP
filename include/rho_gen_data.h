#include "tasep.h"

double *rho_gen_data(uint64_t N, double alpha, double beta,
                     double *(*qf)(TASEP_LAT *), uint32_t r) {
  TASEP_LAT tlat = tasep_init(N);
  tasep_randomize(&tlat, (uint64_t)r);

  double *q = qf(&tlat);
  uint64_t step = tasep_stabilize(&tlat, q, alpha, beta, 10000000000, &r);
  double *rho = tasep_rho(&tlat, q, alpha, beta, step, &r);

  free(q);
  tasep_free(&tlat);
  return rho;
}
