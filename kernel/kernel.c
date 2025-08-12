#include "../drivers/screen.h"

void kernel_main() {
  clear_screen(0);

  put_string_at(10, 10, 1, "ABC CBCB AAAA");
}
