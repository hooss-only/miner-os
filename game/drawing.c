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

#define FLAG_WIDTH 6
#define FLAG_HEIGHT 7
const char FLAG[FLAG_HEIGHT][FLAG_WIDTH] = {
  {0, 0, 4, WHITE, 0, 0},
  {0, 4, 4, WHITE, 0, 0},
  {4, 4, 4, WHITE, 0, 0},
  {0, 4, 4, WHITE, 0, 0},
  {0, 0, 4, WHITE, 0, 0},
  {0, 0, 0, WHITE, 0, 0},
  {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE},
};

#define BOMB_WIDTH 2
#define BOMB_HEIGHT 2
const char BOMB[BOMB_HEIGHT][BOMB_WIDTH] = {
  0
};

void draw_flag(int x, int y);
void draw_bomb(int x, int y);

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
  if (cell.is_open && cell.bomb_cnt != 0)
    put_char_at(x+3, y+2, WHITE, bomb_cnt_str[0]);
  
  if (cell.is_marked && !cell.is_open)
    draw_flag(x+2, y+1);

  if (cell.is_bomb && cell.is_open)
    draw_bomb(x+2, y+2);
}

void draw_flag(int x, int y) {
  for (int i=0; i<FLAG_HEIGHT; i++) {
    for (int j=0; j<FLAG_WIDTH; j++) {
      if (FLAG[i][j] == 0) continue;
      put_pixel_at(x+j, y+i, FLAG[i][j]);
    }
  }
}

void draw_bomb(int x, int y) {
  put_pixel_at(x, y, RED);
}

void draw_rect(int x, int y, int w, int h, unsigned char color) {
  for (int i=0; i<h; i++) {
    for (int j=0; j<w; j++) {
      if (i == 0 || j == 0 || i == h-1 || j == w-1)
        put_pixel_at(x+j, y+i, color);
    }
  }
}
