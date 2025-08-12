#include "../drivers/screen.h"
#include "../game/cell.h"
#include "../game/drawing.h"

void kernel_main() {
  clear_screen(0);

  put_string_at(10, 10, WHITE, "MINER OS");
  
  cell_t cell = {
    .is_bomb = 0,
    .is_open = 0,
  };

  draw_cell_at(20, 20, cell);
  cell.is_open = 1;
  draw_cell_at(20, 30, cell);
}
