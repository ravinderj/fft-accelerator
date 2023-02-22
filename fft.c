#include <complex.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "helper.h"

const double PI = 3.141592653589793238460;

void splitEvenOddPositioned(complex double *data, int size,
                            complex double *even, complex double *odd) {
  int index = 0;
  for (int i = 0; i < size; i++) {
    if (i % 2 == 0) {
      even[index] = data[i];
    } else {
      odd[index] = data[i];
      index++;
    }
  }
}

void fft(complex double *data, int size) {
  if (size <= 1) {
    return;
  }
  complex double even[size / 2];
  complex double odd[size / 2];
  splitEvenOddPositioned(data, size, even, odd);

  fft(even, size / 2);
  fft(odd, size / 2);

  for (int k = 0; k < size / 2; k++) {
    complex double t = odd[k] * cexp(-2I * PI * k / size);
    data[k] = even[k] + t;
    data[k + size / 2] = even[k] - t;
  }
}

int main(int argc, char const *argv[]) {
  struct timeval start, end;
  uint32_t t_beg, t_end;

  int size = 8;
  complex double data[] = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0};

#ifdef RDCYCLE
  t_beg = rdcyc();
#endif
  gettimeofday(&start, NULL);
  fft(data, size);
  gettimeofday(&end, NULL);
#ifdef RDCYCLE
  t_end = rdcyc();
  printf("Cycle count: %d\n", t_end - t_beg);
#endif

  for (int i = 0; i < size; i++) {
    printf("(%.2f %.2f),", creal(data[i]), cimag(data[i]));
  }
  printf("\nTime taken: %dµsec\n", end.tv_usec - start.tv_usec);
  return 0;
}
