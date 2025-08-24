#ifndef GAME_H
#define GAME_H

#include "cell.h"

#define MAX_WIDTH 30
#define MAX_HEIGHT 15

typedef struct {
  unsigned int w;
  unsigned int h;
  unsigned int mines;
  int pane_x;
  int pane_y;
  int sel_x;
  int sel_y;
} game_status_t;

extern game_status_t game_status;
extern cell_t pane[MAX_HEIGHT][MAX_WIDTH];

extern const int dx[8];
extern const int dy[8];

void init_game(unsigned int w, unsigned int h, unsigned int mines);

#endif
