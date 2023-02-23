#include "helper.h"
#include <complex.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

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
  if (argc < 2) {
    printf("needs an argument. int size\n");
    exit(1);
  }

  int size = atoi(argv[1]);
  complex double data[size];
  complex double res[size];
  genSineWave(size, data);

#ifdef RDCYCLE
  t_beg = rdcyc();
#endif
  clock_t start, end;
  start = clock();
  dft(data, size, res);
  end = clock();
#ifdef RDCYCLE
  t_end = rdcyc();
  printf("Cycle count: %d\n", t_end - t_beg);
#endif

  printf("DFT,%d,%.f µsec\n", size,
         1000000 * (double)(end - start) / (double)CLOCKS_PER_SEC);
  return 0;
}