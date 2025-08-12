#include "menu.h"

#include "../cpu/ports.h"
#include "../cpu/irq.h"
#include "../drivers/screen.h"

int sel = 0;

void draw_menu();

#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
static void keyboard_handle(registers_t regs) {
  unsigned char scancode = port_byte_in(0x60);

  if (scancode == ARROW_UP) sel--;
  else if (scancode == ARROW_DOWN) sel++;

  if (sel < 0) sel = 0;
  if (sel > 2) sel = 2;

  draw_menu();
}

/* PUBLIC FUNCTIONS */
void init_menu() {
  draw_menu();
  
  register_interrupt_handler(IRQ1, keyboard_handle);
}

/* PRIVATE FUNCTIONS */

const char* MENU[] = {
  "EASY",
  "NORMAL",
  "HARD"
};

void draw_menu() {
  clear_screen(0);
  put_string_at(10, 10, WHITE, "MINER OS");
  put_string_at(10, 20, LIGHT_GRAY, "A 32BITS OPERATING SYSTEM");
  put_string_at(10, 30, LIGHT_GRAY, "ONLY FOR MINESWEEPER GAME");

  for (int i=0; i<3; i++) {
    char color = WHITE;
    if (sel == i) color = LIGHT_YELLOW;
    put_string_at(15, 45+i*10, color, MENU[i]);
  }
}
