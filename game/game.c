#include "game.h"

#include "../drivers/screen.h"

#include "../cpu/irq.h"
#include "../cpu/ports.h"

#include "cell.h"
#include "drawing.h"

game_status_t game_status = { 0 };

cell_t pane[MAX_HEIGHT][MAX_WIDTH] = { 0 };

void draw_game();

#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ARROW_LEFT 0x4B
#define ARROW_RIGHT 0x4D

#define KEY_X 0x2D
#define KEY_C 0x2E

void move_cursor(unsigned char scancode) {
  switch (scancode) {
    case ARROW_UP: game_status.sel_y--; break;
    case ARROW_DOWN: game_status.sel_y++; break;
    case ARROW_LEFT: game_status.sel_x--; break;
    case ARROW_RIGHT: game_status.sel_x++; break;
  }

  if (game_status.sel_x < 0) game_status.sel_x = 0;
  else if (game_status.sel_x >= game_status.w)
    game_status.sel_x = game_status.w-1;

  if (game_status.sel_y < 0) game_status.sel_y = 0;
  else if (game_status.sel_y >= game_status.h)
    game_status.sel_y = game_status.h-1;
}

static void keyboard_handle(registers_t regs) {
  unsigned char scancode = port_byte_in(0x60);
  
  move_cursor(scancode);
  
  draw_game();
}

void init_game(unsigned int w, unsigned int h, unsigned int mines) {
  game_status.w = w;
  game_status.h = h;
  game_status.mines = mines;

  game_status.pane_x = 10;
  game_status.pane_y = 10;

  for (int i=0; i<(int)h; i++) {
    for (int j=0; j<(int)w; j++) {
      pane[i][j].bomb_cnt = 0;
      pane[i][j].is_bomb = 0;
      pane[i][j].is_open = 0;
    }
  }

  draw_game();

  register_interrupt_handler(IRQ1, keyboard_handle);
}

void draw_cursor() {
  int x = game_status.pane_x, y = game_status.pane_y;
  x += game_status.sel_x * CELL_SIZE;
  y += game_status.sel_y * CELL_SIZE;
  draw_rect(x, y, CELL_SIZE, CELL_SIZE, WHITE);
}

void draw_game() {
  clear_screen(0);

  draw_pane(pane, game_status);
  
  draw_cursor();
}
