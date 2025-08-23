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
#include "logger.h"
#include "menu.h"

game_status_t game_status = { 0 };

cell_t pane[MAX_HEIGHT][MAX_WIDTH] = { 0 };

char visited[MAX_HEIGHT][MAX_WIDTH] = { 0 };
stack_point_t s;

const int dx[] = {0, 0, 1, 1, 1, -1, -1, -1};
const int dy[] = {1, -1, 1, -1, 0, 1, -1, 0};

int marks = 0;

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

#define KEY_R 0x13
#define KEY_X 0x2D
#define KEY_C 0x2E
#define KEY_B 0x30

static void keyboard_handle(registers_t regs) {
  unsigned char scancode = port_byte_in(0x60);
  
  move_cursor(scancode);

  if (scancode == KEY_R) // restart
    init_menu();

  if (scancode == KEY_X) // mark
    mark();

  if (scancode == KEY_C) // open
    open(game_status.sel_x, game_status.sel_y);

  if (scancode == KEY_B) // toogle_debug_log;
    toggle_log();
  
  draw_game();
}

void init_game(unsigned int w, unsigned int h, unsigned int mines) {
  game_status.w = w;
  game_status.h = h;
  game_status.mines = mines;
  marks = 0;

  init_logger();
  
  set_random_seed(get_tick());

  init_pane();

  draw_game();

  append_log("WELCOME TO DEBUG LOGGER", LOG_NORMAL);

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

void install_bombs() {
  stack_point_init(&s);
  int x, y;

  // generate unique positions for mines
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
  // install bomb
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

  draw_log();
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
      "ARROWS TO MOVE   X TO MARK   C TO OPEN"
  );

  put_string_at(
      anchor_x,
      anchor_y + 1 * (FONT_HEIGHT + 2),
      WHITE,
      "R TO RESTART"
  );

  char ascii_buf[10] = { 0 };
  char remain_bombs_and_marks_made[50] = { 0 };

  int_to_ascii(game_status.mines, ascii_buf);
  append_str(remain_bombs_and_marks_made, ascii_buf);
  append_str(remain_bombs_and_marks_made, " MINE");
  if (game_status.mines > 1) append(remain_bombs_and_marks_made, 'S');
  append_str(remain_bombs_and_marks_made, " REMAIN");
  
  if (marks > 0) {
    append_str(remain_bombs_and_marks_made, " AND ");
    int_to_ascii(marks, ascii_buf);
    append_str(remain_bombs_and_marks_made, ascii_buf);
    append_str(remain_bombs_and_marks_made, " MARK");
    if (marks > 1) append(remain_bombs_and_marks_made, 'S');
    append_str(remain_bombs_and_marks_made, " YOU MADE");
  }

  put_string_at(
      anchor_x,
      anchor_y + 2 * (FONT_HEIGHT + 2),
      WHITE,
      remain_bombs_and_marks_made
  );
}

// toggle is_marked of cell at curosr.
void mark() {
  cell_t* cell = &pane[game_status.sel_y][game_status.sel_x];
  if (cell->is_open) return;
  cell->is_marked = !cell->is_marked;
  if (cell->is_marked) marks++;
  else marks--;
}

void open(int x, int y) {
  cell_t* cell = &pane[y][x];

  if (cell->is_bomb) return; // game over

  if (cell->is_open && cell->bomb_cnt != 0) open_near(x, y);

  if (cell->bomb_cnt == 0 && !cell->is_open) open_DFS(x, y);

  cell->is_open = 1;
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
      if (pane[ny][nx].is_bomb) continue; // gameover
      if (pane[ny][nx].is_open) continue;
      if (pane[ny][nx].is_marked) continue;
      if (!pane[ny][nx].is_bomb && pane[ny][nx].bomb_cnt == 0) open_DFS(nx, ny);
      pane[ny][nx].is_open = 1;
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
