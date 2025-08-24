#ifndef DRAWING_H
#define DRAWING_H

#include "cell.h"
#include "../drivers/screen.h"

#include "game.h"

#define CELL_SIZE 10

void draw_cell_at(int x, int y, cell_t cell);
void draw_pane();

void draw_rect(int x, int y, int w, int h, unsigned char color);

#endif
