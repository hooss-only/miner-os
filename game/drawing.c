#include "drawing.h"

#define CELL_SIZE 10
const char CELL[CELL_SIZE][CELL_SIZE] = {
  {28, 28, 28, 28, 28, 28, 28, 28, 28, 26},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {28, 25, 25, 25, 25, 25, 25, 25, 25, 20},
  {26, 20, 20, 20, 20, 20, 20, 20, 20, 20},
};

void draw_cell_at(int x, int y, cell_t cell) {
  if (!cell.is_open) {
    for (int i=0; i<CELL_SIZE; i++) {
      for (int j=0; j<CELL_SIZE; j++) {
        put_pixel_at(x+i, y+j, CELL[j][i]);
      }
    }
    return;
  }
  for (int i=0; i<CELL_SIZE; i++) {
      for (int j=0; j<CELL_SIZE; j++) {
        put_pixel_at(x+i, y+j, CELL[j][i] - 3);
        put_char_at(x+3, y+2, WHITE, 'A');
      }
    }
}
