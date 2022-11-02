#include "rho_gen_data.h"

double *q_choice_1(TASEP_LAT *tlat) {
  double *q = (double *)calloc(tlat->N, sizeof(double));
  double dx = 1.0 / (double)(tlat->N - 1), x = 0;
  for (uint64_t k = 0; k < tlat->N; k++) {
    if (x <= 0.5) {
      q[k] = 1 / (1 + 2 * x);
    } else {
      q[k] = 1 / (3 - 2 * x);
    }
    x += dx;
  }
  return q;
}

int main(void) {
  uint32_t r = (uint32_t)time(0);
  # pragma omp parallel for
  for (uint32_t i=0; i < 100; i++) {
     char buff[512];
     snprintf(buff, 512, "raw_data/rho_q1/q_choice_1_a%f_b%f_%d.csv", 0.1, 0.7, r + 248420 * i);
     rho_gen_data(1000, 0.1, 0.7, q_choice_1, buff, r + 248420 * i);
     printf("%s\n", buff);
  }
}
