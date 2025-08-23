#include "math.h"

int random_seed;

int randint(int min, int max) {
  const unsigned int a = 1664525;
  const unsigned int c = 1013904223;

  random_seed = (a * random_seed + c);
  unsigned int r = (unsigned int) random_seed;
  int range = max - min + 1;
  return min + (r % range);
}

void set_random_seed(int seed) {
  random_seed = seed;
}
