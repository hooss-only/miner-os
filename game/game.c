#include "game.h"

#include "../drivers/screen.h"

#include "../cpu/irq.h"
#include "../cpu/ports.h"
#include "../cpu/timer.h"

#include "../libc/string.h"
#include "../libc/stack.h"
#include "../libc/math.h"

#include "cell.h"
#include "drawing.h"

game_status_t game_status = { 0 };

cell_t pane[MAX_HEIGHT][MAX_WIDTH] = { 0 };

char visited[MAX_HEIGHT][MAX_WIDTH] = { 0 };
stack_point_t s;

const int dx[] = {0, 0, 1, 1, 1, -1, -1, -1};
const int dy[] = {1, -1, 1, -1, 0, 1, -1, 0};

void init_pane();
void install_bombs();

void reset_visited();

void draw_game();
void draw_cursor();
void draw_ui();

void move_cursor(unsigned char scancode);

void open(int x, int y);
void mark();
unsigned int near_bomb_cnt(int x, int y);
unsigned int near_mark_cnt(int x, int y);

void open_near(int x, int y);
void open_DFS(int x, int y);

#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ARROW_LEFT 0x4B
#define ARROW_RIGHT 0x4D

#define KEY_X 0x2D
#define KEY_C 0x2E

static void keyboard_handle(registers_t regs) {
  unsigned char scancode = port_byte_in(0x60);
  
  move_cursor(scancode);

  if (scancode == KEY_X) // mark
    mark();

  if (scancode == KEY_C) // open
    open(game_status.sel_x, game_status.sel_y);
  
  draw_game();
}

void init_game(unsigned int w, unsigned int h, unsigned int mines) {
  game_status.w = w;
  game_status.h = h;
  game_status.mines = mines;
  
  set_random_seed(get_tick());

  init_pane();

  draw_game();

  register_interrupt_handler(IRQ1, keyboard_handle);
}

void init_pane() {
  game_status.pane_x = 10;
  game_status.pane_y = 10;

  install_bombs();

  for (int i=0; i<(int)game_status.h; i++) {
    for (int j=0; j<(int)game_status.w; j++) {
      pane[i][j].bomb_cnt = near_bomb_cnt(j, i);
      pane[i][j].is_open = 0;
      pane[i][j].is_marked = 0;
    }
  }

}

// TODO: Fix bomb is not enough
void install_bombs() {
  stack_point_init(&s);
  int x, y;
  while (s.length < (int) game_status.mines) {
    char same_pos = 0;
    x = randint(0, game_status.w-1);
    y = randint(0, game_status.h-1);
    for (int i=0; i<s.length; i++) {
      if (s.array[i].x == x && s.array[i].y == y) {
        same_pos = 1;
        break;
      }
    }

    if (same_pos) continue;
    stack_point_push(&s, x, y);
  }

  while (s.length) {
    stack_point_pop(&s, &x, &y);
    pane[y][x].is_bomb = 1;
  }
}

void reset_visited() {
  for (int i=0; i<(int)game_status.h; i++) {
    for (int j=0; j<(int)game_status.w; j++) {
      visited[i][j] = 0;
    }
  }
}

void draw_game() {
  clear_screen(0);

  draw_pane(pane, game_status);

  draw_ui();
  
  draw_cursor();
}

void move_cursor(unsigned char scancode) {
  switch (scancode) {
    case ARROW_UP: game_status.sel_y--; break;
    case ARROW_DOWN: game_status.sel_y++; break;
    case ARROW_LEFT: game_status.sel_x--; break;
    case ARROW_RIGHT: game_status.sel_x++; break;
  }

  if (game_status.sel_x < 0) game_status.sel_x = 0;
  else if (game_status.sel_x >= (int) game_status.w)
    game_status.sel_x = (int) game_status.w-1;

  if (game_status.sel_y < 0) game_status.sel_y = 0;
  else if (game_status.sel_y >= (int) game_status.h)
    game_status.sel_y = (int) game_status.h-1;
}

void draw_cursor() {
  int x = game_status.pane_x, y = game_status.pane_y;
  x += game_status.sel_x * CELL_SIZE;
  y += game_status.sel_y * CELL_SIZE;
  draw_rect(x, y, CELL_SIZE, CELL_SIZE, WHITE);
}

void draw_ui() {
  int anchor_x = game_status.pane_x;
  int anchor_y = game_status.pane_y + MAX_HEIGHT*CELL_SIZE + 5;

  char bomb_amount[10] = { 0 };
  
  int_to_ascii(game_status.mines, bomb_amount);
  put_string_at(
      anchor_x,
      anchor_y,
      WHITE,
      bomb_amount
  );
}

// toggle is_marked of cell at curosr.
void mark() {
  pane[game_status.sel_y][game_status.sel_x].is_marked = !pane[game_status.sel_y][game_status.sel_x].is_marked;
}

void open(int x, int y) {
  cell_t* cell = &pane[y][x];

  if (cell->is_bomb) return; // game over

  if (cell->is_open && cell->bomb_cnt != 0) open_near(x, y);

  if (cell->bomb_cnt == 0 && !cell->is_open) open_DFS(x, y);

  cell->is_open = 1;
}

void open_near(int x, int y) {
  cell_t* cell = &pane[y][x];

  if (near_mark_cnt(x, y) >= cell->bomb_cnt) {
    int nx, ny;
    for (int i=0; i<8; i++) {
      nx = x + dx[i];
      ny = y + dy[i];
      if (nx < 0 || nx >= (int) game_status.w || ny < 0 || ny >= (int) game_status.h) continue;
      if (pane[ny][nx].is_marked) continue;
      open(nx, ny);
    }
  }
}

void open_DFS(int x, int y) {
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
