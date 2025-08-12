#include "screen.h"

char alphabet[3][7][7] = {
  {
    {0, 1, 1, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {1, 1, 1, 1, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
  },
  {
    {1, 1, 1, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0},
  },
  {
    {0, 1, 1, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0},
  }
};

int put_pixel_at(int x, int y, unsigned char color) {
  if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return -1;
  unsigned char* vmem = (unsigned char*) VIDEO_MEM;

  vmem[y * SCREEN_WIDTH + x] = color;

  return 0;
}

void put_char_at(int x, int y, unsigned char color, char c) {
  for (int i=0; i<7; i++) {
    for (int j=0; j<7; j++) {
      if (!alphabet[c-'A'][j][i]) continue;
      put_pixel_at(x+i, y+j, color);
    }
  }
}

void clear_screen(unsigned char color) {
  int screen_size = SCREEN_WIDTH * SCREEN_HEIGHT;
  unsigned char* vmem = (unsigned char*) VIDEO_MEM;

  for (int i=0; i<screen_size; i++) {
    vmem[i] = color;
  }
}
