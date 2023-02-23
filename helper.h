#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <complex.h>

static inline uint32_t rdcyc() {
  uint32_t val;
  asm volatile("rdcycle %0 ;\n" : "=r"(val)::);
  return val;
}

void linspace(int start, int stop, uint num, _Complex double *res) {
  float interval = (stop - start + 1) / (float)num;

  int counter = 0;
  for (float i = start; i <= stop; i += interval) {
    res[counter] = i;
    counter++;
  }
}

void genSineWave(int size, _Complex double *data) {
  linspace(1, 10, size, data);
  for (int i = 0; i < size; i++) {
    data[i] = csin(data[i]);
  }
}
