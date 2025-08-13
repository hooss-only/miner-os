#include "math.h"

int random_seed;

int randint(int min, int max) {
  const unsigned int a = 1664525;
  const unsigned int c = 1013904223;

  random_seed = (a * random_seed + c);
  int range = max - min + 1;
  return min + (random_seed % range);
}

void set_randome_seed(int seed) {
  random_seed = seed;
}
