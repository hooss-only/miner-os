#ifndef STACK_H
#define STACK_H

#include "../game/game.h"

#define MAX_STACK_SIZE MAX_WIDTH * MAX_HEIGHT

typedef struct {
  int x;
  int y;
} point_t;

typedef struct {
  int length;
  point_t array[MAX_STACK_SIZE];
} stack_point_t;

void stack_point_init(stack_point_t* stack);
void stack_point_push(stack_point_t* stack, int x, int y);
void stack_point_pop(stack_point_t* stack, int* x, int* y);

#endif
