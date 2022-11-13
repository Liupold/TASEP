#include <stdio.h>
#include <stdint.h>

typedef double (*qf_ptr)(double);

double q0(double x) {
  (void) x;
  return 1;
}

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

qf_ptr choose_q(uint32_t q_num) {
  if (q_num == 0) {
    return q0;
  } else if (q_num == 1) {
    return q1;
  } else if (q_num == 2) {
    return q2;
  }
  return NULL;
}
