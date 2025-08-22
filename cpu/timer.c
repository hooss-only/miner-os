#include "timer.h"

#include "irq.h"
#include "ports.h"

unsigned int tick = 0;

static void timer_callback(registers_t regs) {
  tick++;
}

unsigned int get_tick() { return tick; }

void init_timer(int freq) {
  register_interrupt_handler(IRQ0, timer_callback);

  int divisor = 1193180 / freq;
  char low = (char) (divisor & 0xFF);
  char high = (char) ((divisor>>8) & 0xFF);

  port_byte_out(0x43, 0x36);
  port_byte_out(0x40, low);
  port_byte_out(0x40, high);
}
