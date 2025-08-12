#ifndef CELL_H
#define CELL_H

typedef struct {
  char is_open;
  char is_bomb;
  unsigned int bomb_cnt;
} cell_t;

#endif
