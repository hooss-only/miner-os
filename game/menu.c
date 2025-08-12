#include "menu.h"

#include "../cpu/ports.h"
#include "../cpu/irq.h"
#include "../drivers/screen.h"

#define MENU_LENGTH 3
int sel = 0;

void draw_menu();

#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
static void keyboard_handle(registers_t regs) {
  unsigned char scancode = port_byte_in(0x60);

  if (scancode == ARROW_UP) sel--;
  else if (scancode == ARROW_DOWN) sel++;

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

const char* MENU[MENU_LENGTH] = {
  "EASY   10 X 10 10 MINES",
  "NORMAL 20 X 20 40 MINES",
  "HARD   40 X 20 100 MINES"
};

void draw_menu() {
  clear_screen(0);
  put_string_at(10, 10, WHITE, "MINER OS");
  put_string_at(10, 20, LIGHT_GRAY, "A 32BITS OPERATING SYSTEM");
  put_string_at(10, 30, LIGHT_GRAY, "ONLY FOR MINESWEEPER GAME");

  for (int i=0; i<MENU_LENGTH; i++) {
    char color = WHITE;
    if (sel == i) color = LIGHT_YELLOW;
    put_string_at(15, 45+i*10, color, (char*) MENU[i]);
  }
}
