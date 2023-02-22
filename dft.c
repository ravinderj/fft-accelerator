#include <complex.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>
#include "helper.h"

void dft(complex double *data, int size, complex double *res) {
    const double PI = 3.141592653589793238460;

    for (int freq = 0; freq < size; freq++) {
        complex double r = 0;
        for (int t = 0; t < size; t++) {
            r += data[t] * cexp(-2I * PI * freq * t / size);
        }
        res[freq] = r;
    }
}

int main(int argc, const char **argv) {
  struct timeval start, end;
  uint32_t t_beg, t_end;

  int size = 8;
  complex double data[] = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0};
  complex double res[size];

#ifdef RDCYCLE
  t_beg = rdcyc();
#endif
  gettimeofday(&start, NULL);
  dft(data, size, res);
  gettimeofday(&end, NULL);
#ifdef RDCYCLE
  t_end = rdcyc();
  printf("Cycle count: %d\n", t_end - t_beg);
#endif

  for (int i = 0; i < size; i++) {
    printf("(%.2f %.2f),", creal(res[i]), cimag(res[i]));
  }
  printf("\nTime taken: %dµsec\n", end.tv_usec - start.tv_usec);
  return 0;
}