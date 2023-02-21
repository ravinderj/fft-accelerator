#include <complex.h>
#include <math.h>
#include <stdio.h>

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
    data[k]            = even[k] + t;
    data[k + size / 2] = even[k] - t;
  }
}

int main(int argc, char const *argv[]) {
  complex double data[8] = {1, 1, 1, 1, 0, 0, 0, 0};
  int size = sizeof(data) / sizeof(complex double);

  fft(data, size);

  show(" ", data);

  return 0;
}
