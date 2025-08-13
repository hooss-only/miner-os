#ifndef GAME_H
#define GAME_H

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

void init_game(unsigned int w, unsigned int h, unsigned int mines);

#endif
