#include "menu.h"

#include "../cpu/ports.h"
#include "../cpu/irq.h"
#include "../drivers/screen.h"

#include "game.h"

#define MENU_LENGTH 3
int sel = 0;

struct menu {
  const char* name;
  unsigned int w;
  unsigned int h;
  unsigned int mines;
};

const struct menu MENU[MENU_LENGTH] = {
  {
    .name = "EASY 10 MINES 10X10",
    .w = 10,
    .h = 10,
    .mines = 10
  },
  {
    .name = "NORMAL 20 MINES 20X20",
    .w = 20,
    .h = 20,
    .mines = 40
  },
  {
    .name = "HARD 100 MINES 40X20",
    .w = 40,
    .h = 20,
    .mines = 100
  }
};

void draw_menu();

#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ENTER 0x1C
static void keyboard_handle(registers_t regs) {
  unsigned char scancode = port_byte_in(0x60);

  if (scancode == ARROW_UP) sel--;
  else if (scancode == ARROW_DOWN) sel++;
  else if (scancode == ENTER) init_game(MENU[sel].w, MENU[sel].h, MENU[sel].mines);

  if (sel < 0) sel = 0;
  if (sel >= MENU_LENGTH) sel = MENU_LENGTH - 1;

  draw_menu();
}

/* PUBLIC FUNCTIONS */
void init_menu() {
  draw_menu();
  
  register_interrupt_handler(IRQ1, keyboard_handle);
}

/* PRIVATE FUNCTIONS */
void draw_menu() {
  clear_screen(0);
  put_string_at(10, 10, WHITE, "MINER OS BY HOOSS_ONLY");
  put_string_at(10, 20, LIGHT_GRAY, "A 32BITS OPERATING SYSTEM");
  put_string_at(10, 30, LIGHT_GRAY, "ONLY FOR MINESWEEPER GAME");

  for (int i=0; i<MENU_LENGTH; i++) {
    char color = WHITE;
    if (sel == i) color = LIGHT_YELLOW;
    put_string_at(15, 45+i*10, color, (char*) MENU[i].name);
  }
}
