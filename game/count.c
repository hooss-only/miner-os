#include "count.h"
#include "cell.h"

unsigned int near_bomb_cnt(int x, int y) {
  unsigned int result = 0;
  
  int nx, ny;
  cell_t* cell;
  for (int i=0; i<8; i++) {
    nx = x + dx[i];
    ny = y + dy[i];
    if (nx < 0 || nx >= (int) game_status.w || ny < 0 || ny >= (int) game_status.h) continue;
    cell = &pane[ny][nx];

    if (cell->is_bomb) result++;
  }

  return result;
}

unsigned int near_mark_cnt(int x, int y) {
  unsigned int result = 0;
  
  int nx, ny;
  cell_t* cell;
  for (int i=0; i<8; i++) {
    nx = x + dx[i];
    ny = y + dy[i];
    if (nx < 0 || nx >= (int) game_status.w || ny < 0 || ny >= (int) game_status.h) continue;
    cell = &pane[ny][nx];

    if (cell->is_marked) result++;
  }

  return result;
}
