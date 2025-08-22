#include "../cpu/irq.h"
#include "../cpu/idt.h"
#include "../cpu/timer.h"

#include "../game/menu.h"

void kernel_main() {
  irq_install();
  set_idt();

  init_timer(50);

  init_menu();
}
