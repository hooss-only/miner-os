#include "open.h"
#include "game.h"
#include "count.h"
#include "../libc/stack.h"

const int dx[] = {0, 0, 1, 1, 1, -1, -1, -1};
const int dy[] = {1, -1, 1, -1, 0, 1, -1, 0};

void open(int x, int y) {
  cell_t* cell = &pane[y][x];

  if (cell->is_bomb) gameover();

  if (cell->is_open && cell->bomb_cnt != 0) open_near(x, y);

  if (cell->bomb_cnt == 0 && !cell->is_open) open_DFS(x, y);

  cell->is_open = 1;
  
  check_win();
}

// TODO: game over when near is bomb
void open_near(int x, int y) {
  cell_t* cell = &pane[y][x];

  if (near_mark_cnt(x, y) >= cell->bomb_cnt) {
    int nx, ny;
    for (int i=0; i<8; i++) {
      nx = x + dx[i];
      ny = y + dy[i];
      if (nx < 0 || nx >= (int) game_status.w || ny < 0 || ny >= (int) game_status.h) continue;
      if (pane[ny][nx].is_open) continue;
      if (pane[ny][nx].is_marked) continue;
      if (pane[ny][nx].is_bomb) gameover();
      if (!pane[ny][nx].is_bomb && pane[ny][nx].bomb_cnt == 0) open_DFS(nx, ny);
      pane[ny][nx].is_open = 1;
    }
  }
}

char visited[MAX_HEIGHT][MAX_WIDTH] = { 0 };

void reset_visited() {
  for (int i=0; i<(int)game_status.h; i++) {
    for (int j=0; j<(int)game_status.w; j++) {
      visited[i][j] = 0;
    }
  }
}

void open_DFS(int x, int y) {
  stack_point_t s;
  cell_t* cell;
  stack_point_init(&s);
  reset_visited();

  visited[y][x] = 1;

  stack_point_push(&s, x, y);
  
  int nx, ny;
  while (s.length) {
    stack_point_pop(&s, &x, &y);
    cell = &pane[y][x];
    cell->is_open = 1;
    
    for (int i=0; i<8; i++) {
      nx = x+dx[i];
      ny = y+dy[i];
      if (nx < 0 || nx >= (int) game_status.w || ny < 0 || ny >= (int) game_status.h) continue;
      if (pane[ny][nx].bomb_cnt != 0) {
        open(nx, ny);
        continue;
      }
      if (visited[ny][nx]) continue;
      stack_point_push(&s, nx, ny);
      visited[ny][nx] = 1;
    }
  };
}
