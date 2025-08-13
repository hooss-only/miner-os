#include "../cpu/irq.h"
#include "../cpu/idt.h"

#include "../game/menu.h"

void kernel_main() {
  irq_install();
  set_idt();

  init_menu();
}
