#include <complex.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

const double PI = 3.141592653589793238460;

void show(const char *s, complex double buf[]) {
  printf("%s", s);
  for (int i = 0; i < 8; i++)
    if (!cimag(buf[i]))
      printf("%g ", creal(buf[i]));
    else
      printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
}

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
  if (size <= 1)
    return;
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

static inline uint32_t rdcyc() {
  uint32_t val;
  asm volatile ("rdcycle %0 ;\n":"=r" (val) ::);
  return val;
}

void genInput(int size, complex double *in) {
  srand(time(NULL));
  for (int i = 0; i < size; i++) {
    in[i] = rand() % 10;
  }
}

int main(int argc, char const *argv[]) {

  if (argc < 2) {
    printf("usage: <executable> size [-DRDCYCLE]\n");
    return 1;
  }

  int size = atoi(argv[1]);
  struct timeval start, end;
  uint32_t t_beg, t_end;

  complex double *data = malloc(size * sizeof(int));
  if (data == NULL) {
    printf("Memory not allocate\n");
    exit(1);
  }
  genInput(size, data);

  #ifdef RDCYCLE
  t_beg = rdcyc();
  // host = "RISC-V";
  #endif
  gettimeofday(&start, NULL);
  fft(data, size);
  gettimeofday(&end, NULL);
  #ifdef RDCYCLE
  t_end = rdcyc();
  printf("%d\n", t_end - t_beg);
  #endif

  show(" ", data);
  printf("\nTime taken: %dµsec\n", end.tv_usec - start.tv_usec);
  free(data);
  return 0;
}
