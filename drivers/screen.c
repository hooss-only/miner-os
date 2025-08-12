#include "screen.h"

#define FONT_WIDTH 7
#define FONT_HEIGHT 7

const char ALPHABET[3][FONT_WIDTH][FONT_HEIGHT] = {
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
  for (int i=0; i<FONT_HEIGHT; i++) {
    for (int j=0; j<FONT_WIDTH; j++) {
      if (!ALPHABET[c-'A'][j][i]) continue;
      put_pixel_at(x+i, y+j, color);
    }
  }
}

void put_string_at(int x, int y, unsigned char color, char* str) {
  int dx = 0;
  char c;
  while ((c = *str) != 0) {
    str++;
    dx += FONT_WIDTH;
    put_char_at(x + dx, y, color, c);
  }
}

void clear_screen(unsigned char color) {
  int screen_size = SCREEN_WIDTH * SCREEN_HEIGHT;
  unsigned char* vmem = (unsigned char*) VIDEO_MEM;

  for (int i=0; i<screen_size; i++) {
    vmem[i] = color;
  }
}
