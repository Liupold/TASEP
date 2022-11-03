#define  _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char **argv) {
  char *filename, filename_cpy[512];
  uint64_t N;
  uint32_t seed_avg, q_num;
  double alpha, beta;
  char resp;
  FILE *fpr;
  char *line = NULL;
  size_t len = 0;


  if (argc < 2) {
    fprintf(stderr, "Useage: %s <fiename>\n", argv[0]);
  }

  filename = argv[1];
  strcpy(filename_cpy, filename);
  printf("Reading From: \"%s\"\n", filename);
  sscanf(basename(filename_cpy), "rho-density_q%u_N-%lu_alpha-%lf_beta-%lf_avg-%u.csv",
                  &q_num, &N, &alpha, &beta, &seed_avg);

  printf("Using: N = %lu, alpha = %f, beta = %f\n", N, alpha, beta);
  printf("Using: q%d(x), seed avg = %u\n", q_num, seed_avg);
  printf("Continue? (y/n)");
  scanf("%c", &resp);

  // <---------------------------->

  fpr = fopen(filename, "r");
  while (getline(&line, &len, fpr) != -1) {
        printf("%s", line);
    }
  fclose(fpr);
}
