#include "screen.h"
#include "font.h"
#include "../libc/memory.h"

int put_pixel_at(int x, int y, unsigned char color) {
  if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return -1;
  unsigned char* vmem = (unsigned char*) VIDEO_MEM;

  vmem[y * SCREEN_WIDTH + x] = color;

  return 0;
}

void put_char_at(int x, int y, unsigned char color, char c) {
  char font[FONT_WIDTH][FONT_HEIGHT] = { 0 };
  if (c >= '0' && c <= '9') memory_copy((unsigned char*) &font, (unsigned char*) &NUMBER[c-'0'], FONT_WIDTH*FONT_HEIGHT);
  else if (c >= 'A' && c <= 'Z') memory_copy((unsigned char*) &font, (unsigned char*) &ALPHABET[c-'A'], FONT_WIDTH*FONT_HEIGHT);
  else return;

  for (int i=0; i<FONT_HEIGHT; i++) {
    for (int j=0; j<FONT_WIDTH; j++) {
      if (!font[j][i]) continue;
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
