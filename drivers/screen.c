#include "screen.h"

int put_pixel_at(int x, int y, unsigned char color) {
  if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return -1;
  
  unsigned char* vmem = (unsigned char*) VIDEO_MEM;
  
  vmem[y * SCREEN_WIDTH + x] = color;

  return 0;
}

void clear(unsigned char color) {
  int screen_size = SCREEN_WIDTH * SCREEN_HEIGHT;
  unsigned char* vmem = (unsigned char*) VIDEO_MEM;

  for (int i=0; i<screen_size; i++) {
    vmem[i] = color;
  }
}
