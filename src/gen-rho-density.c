#include "tasep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double q1(double x) {
  if (x <= 0.5) {
    return 1 / (1 + 2 * x);
  } else {
    return 1 / (3 - 2 * x);
  }
}

double q2(double x) {
  if (x <= 0.7) {
    return 0.5 * (2 - x * x / 0.49);
  } else {
    return 0.5 * (2 - (x - 1.4) * (x - 1.4) / 0.49);
  }
}

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

void help(void) {
  printf("gen-rho-density [OPTIONS] ...\n");
  printf(
      "\nGenerate rho-density data from TASEP model for given alpha, beta.\n");
  printf("\nWriten By: Rohn Chatterjee  (https://github.com/liupold)\n\n");

  printf("-dir <Path>\tdir/folder.\t(default = data)\n");
  printf("-N <Integer>\tLattice Size.\t(default = 100 )\n");
  printf("-q <Integer>\tq func number.\t(default = 1   )\n");
  printf("-alpha <float>\tvalue of alpha.\t(default = 0.1 )\n");
  printf("-beta  <float>\tvalue of beta.\t(default = 0.7 )\n");
  printf("-help \t\tshow this message!\n");
}

int main(int argc, char **argv) {
  uint64_t N = 100;
  uint32_t r = 138901483, seed_avg = 100, seed_done = 0, q_num = 1;
  double *rho_sum, *rho, *q, dx, x, alpha = 0.1, beta = 0.7;
  char buf[512], resp, *data_dir = "data";
  FILE *fpr;
  double (*qf)(double);

  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-q") == 0) {
      q_num = (uint32_t)strtoul(argv[i + 1], NULL, 10);
      i++;
      continue;
    };
    if (strcmp(argv[i], "-N") == 0) {
      N = strtoull(argv[i + 1], NULL, 10);
      i++;
      continue;
    };
    if (strcmp(argv[i], "-alpha") == 0) {
      alpha = strtod(argv[i + 1], NULL);
      i++;
      continue;
    };
    if (strcmp(argv[i], "-beta") == 0) {
      beta = strtod(argv[i + 1], NULL);
      i++;
      continue;
    };
    if (strcmp(argv[i], "-dir") == 0) {
      data_dir = argv[i + 1];
      i++;
      continue;
    };
    if (strcmp(argv[i], "-help") == 0) {
      help();
      return 0;
    };
    if (strcmp(argv[i], "--help") == 0) {
      help();
      return 0;
    };
  }

  if (q_num == 1) {
    qf = q1;
  } else if (q_num == 2) {
    qf = q2;
  } else {
    fprintf(stderr, "Invalid q number!");
    return 3;
  }

  snprintf(buf, 512, "%s/rho-density_q-%u_N-%lu_alpha-%f_beta-%f_avg-%u.csv",
           data_dir, q_num, N, alpha, beta, seed_avg);
  printf("Using: N = %lu, alpha = %f, beta = %f\n", N, alpha, beta);
  printf("Using: q%d(x), seed avg = %u\n", q_num, seed_avg);
  printf("Saving to: \"%s\"\n", buf);
  printf("Continue? (y/n)");
  scanf("%c", &resp);

  if (resp != 'y') {
    return 0;
  }

  fpr = fopen(buf, "w");
  if (fpr == NULL) {
    fprintf(stderr, "Error: NULL file pointer");
    return 2;
  }

  rho_sum = (double *)calloc(N, sizeof(double));
  q = (double *)calloc(N, sizeof(double));
  dx = 1.0 / (double)(N - 1);
  x = 0;
  for (uint64_t k = 0; k < N; k++) {
    q[k] = qf(x);
    x += dx;
  }

#pragma omp parallel for
  for (uint32_t i = 0; i < seed_avg; i++) {
    rho = rho_gen_data(N, alpha, beta, q, r + 248420 * i);
    for (uint64_t k = 0; k < N; k++) {
      rho_sum[k] += rho[k];
    }
#pragma omp atomic
    ++seed_done;
    free(rho);
#pragma omp critical
    fprintf(stderr, "Progress: [%u/%u]\r", seed_done, seed_avg);
  }
  fprintf(stderr, "--------Done!-------\n");

  x = dx;
  for (uint64_t k = 1; k < N - 1; k++) {
    fprintf(fpr, "%f,%f,%f\n", x, q[k], rho_sum[k] / seed_avg);
    x += dx;
  }

  fclose(fpr);
  free(q);
  free(rho_sum);
}
