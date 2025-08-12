#include "memory.h"

void memory_copy(unsigned char* dest, unsigned char* source, int nbytes) {
  for (int i=0; i<nbytes; i++) {
    *(dest + i) = *(source + i);
  }
}
