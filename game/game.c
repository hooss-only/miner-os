#include "game.h"

#include "../drivers/screen.h"

#include "../cpu/irq.h"
#include "../cpu/ports.h"

void draw_game();

static void keyboard_handle(registers_t regs) {
  unsigned char scancode = port_byte_in(0x60);
  draw_game();
}

void init_game(unsigned int w, unsigned int h, unsigned int mines) {
  draw_game();

  register_interrupt_handler(IRQ1, keyboard_handle);
}

void draw_game() {
  clear_screen(0);
}
