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
#include "count.h"
#include "open.h"

game_status_t game_status = { 0 };

cell_t pane[MAX_HEIGHT][MAX_WIDTH] = { 0 };

int marks = 0;

char pause = 0;

typedef enum {
  RUNNING,
  WIN,
  GAMEOVER,
} status_t;
status_t status = RUNNING;

void init_pane();
void install_bombs();

void draw_game();
void draw_cursor();
void draw_ui();

void move_cursor(unsigned char scancode);

void mark();

void win();


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

  if (pause) return;

  if (scancode == KEY_X) // mark
    mark();

  if (scancode == KEY_C) // open
    open(game_status.sel_x, game_status.sel_y);

  if (scancode == KEY_B) { // toogle_debug_log
    pause = !pause; // toggle pause
    toggle_log();
  }
  
  draw_game();
}

void init_game(unsigned int w, unsigned int h, unsigned int mines) {
  game_status.w = w;
  game_status.h = h;
  game_status.mines = mines;
  marks = 0;
  pause = 0;
  status = RUNNING;

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
  stack_point_t s;
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



void draw_game() {
  clear_screen(0);

  draw_pane(pane, game_status);

  draw_ui();
  
  draw_cursor();

  draw_log();
}

void move_cursor(unsigned char scancode) {
  if (pause) return;
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
  char color = WHITE;
  
  int_to_ascii(game_status.mines, bomb_amount);
  put_string_at(
      anchor_x,
      anchor_y,
      WHITE,
      "ARROWS TO MOVE   X TO MARK   C TO OPEN"
  );
  
  if (pause) color = LIGHT_YELLOW;
  put_string_at(
      anchor_x,
      anchor_y + 1 * (FONT_HEIGHT + 2),
      color,
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

  check_win();
}

/* 
IF marks == mines and every mines marked
*/
void check_win() {
  if (marks != (int) game_status.mines) return;
  
  char r = 0;
  for (int i=0; i<(int) game_status.h; i++) {
    for (int j=0; j<(int) game_status.w; j++) {
      if (!pane[i][j].is_bomb) continue;
      if (!pane[i][j].is_marked) { r = 1; break; }
    }
  }

  if (!r) win();
}

/*
Open cells that is not marked
*/
void win() {
  pause = 1;
  status = WIN;

  for (int i=0; i<(int) game_status.h; i++) {
    for (int j=0; j<(int) game_status.w; j++) {
      if (pane[i][j].is_marked) continue;
      pane[i][j].is_open = 1;
    }
  }
}

// Open every cells that is bomb
void gameover() {
  pause = 1;
  status = GAMEOVER;
  for (int i=0; i<(int) game_status.h; i++) {
    for (int j=0; j<(int) game_status.w; j++) {
      if (pane[i][j].is_bomb) pane[i][j].is_open = 1;
    }
  }
}
