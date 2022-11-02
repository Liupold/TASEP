#include "tasep.h"
#include <stdio.h>
#include <string.h>

void rho_gen_data(uint64_t N, double alpha, double beta,
                  double *(*qf)(TASEP_LAT *), char *filename, uint32_t r) {
  TASEP_LAT tlat = tasep_init(N);
  tasep_randomize(&tlat, (uint64_t)r);
  (void)filename;

  double *q = qf(&tlat);
  uint64_t step = tasep_stabilize(&tlat, q, alpha, beta, 10000000000, &r);
  double *rho = tasep_rho(&tlat, q, alpha, beta, step, &r);

  FILE *fpr = fopen(filename, "w");
  if (fpr == NULL) {
    fprintf(stderr, "File I/O error.\n");
    exit(2);
  }

  double dx = 1.0 / (double)(tlat.N - 1), x = 0 + dx;
  for (uint64_t k = 0; k < tlat.N - 2; k++) {
    fprintf(fpr, "%le,%le,%le\n", x, q[k], rho[k]);
    x += dx;
  }

  fclose(fpr);
  free(q);
  free(rho);
  tasep_free(&tlat);
}
