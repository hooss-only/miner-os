#include "../drivers/screen.h"

void kernel_main() {
  clear_screen(0);

  put_char_at(10, 10, 1, 'A');
  put_char_at(18, 10, 1, 'B');
  put_char_at(18+8, 10, 1, 'C');
}
