#include "menu.h"

#include "../drivers/screen.h"

void draw_menu();

/* PUBLIC FUNCTIONS */
void init_menu() {
  draw_menu();
}

/* PRIVATE FUNCTIONS */
void draw_menu() {
  clear_screen(0);
  put_string_at(10, 10, WHITE, "MINER OS");
  put_string_at(10, 20, LIGHT_GRAY, "A 32BITS OPERATING SYSTEM");
  put_string_at(10, 30, LIGHT_GRAY, "ONLY FOR MINESWEEPER GAME");

  put_string_at(15, 45, LIGHT_YELLOW, "EASY   10 X 10");
  put_string_at(15, 55, WHITE, "NORMAL 15 X 15");
  put_string_at(15, 65, WHITE, "HARD   20 X 20");
}
