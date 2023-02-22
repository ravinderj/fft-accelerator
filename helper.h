#include <stdint.h>

static inline uint32_t rdcyc() {
  uint32_t val;
  asm volatile("rdcycle %0 ;\n" : "=r"(val)::);
  return val;
}