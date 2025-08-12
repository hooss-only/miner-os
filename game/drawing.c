#include "drawing.h"
#include "../libc/string.h"

const char CELL[CELL_SIZE][CELL_SIZE] = {
  {28, 28, 28, 28, 28, 28, 28, 28, 28, 26},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {26, 20, 20, 20, 20, 20, 20, 20, 20, 20},
};

void draw_pane(cell_t pane[][MAX_WIDTH], game_status_t game_status) {
  for (int i=0; i<(int)game_status.h; i++) {
    for (int j=0; j<(int)game_status.w; j++) {
      draw_cell_at(
        game_status.pane_x+j*CELL_SIZE,
        game_status.pane_y+i*CELL_SIZE,
        pane[i][j]
      );
    }
  }
}

void draw_cell_at(int x, int y, cell_t cell) {
  int brightness = 0;
  if (cell.is_open) {
    brightness = -3;
  }

  for (int i=0; i<CELL_SIZE; i++) {
    for (int j=0; j<CELL_SIZE; j++) {
      put_pixel_at(x+i, y+j, CELL[j][i] + brightness);
    }
  }

  char bomb_cnt_str[2] = { 0 };
  int_to_ascii(cell.bomb_cnt, bomb_cnt_str);
  if (cell.is_open) put_char_at(x+3, y+2, WHITE, bomb_cnt_str[0]);
}

void draw_rect(int x, int y, int w, int h, unsigned char color) {
  for (int i=0; i<h; i++) {
    for (int j=0; j<w; j++) {
      if (i == 0 || j == 0 || i == h-1 || j == w-1)
        put_pixel_at(x+j, y+i, color);
    }
  }
}
