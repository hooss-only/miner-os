#include "screen.h"

void clear_text() {
  int screen_size = TEXT_HEIGHT * TEXT_WIDTH;
  volatile char* screen = (volatile char*) VIDEO_MEM_TEXT;

  for (int i=0; i<screen_size; i++) {
    screen[i*2] = ' ';
    screen[i*2+1] = 0x0f;
  }
}
