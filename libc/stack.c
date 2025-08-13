#include "stack.h"

void stack_point_init(stack_point_t* stack) {
  stack->length = 0;
}

void stack_point_push(stack_point_t* stack, int x, int y) {
  if (stack->length == MAX_STACK_SIZE) return; 
  stack->length++;
  stack->array[stack->length - 1].x = x;
  stack->array[stack->length - 1].y = y;
}

void stack_point_pop(stack_point_t* stack, int* x, int* y) {
  if (stack->length == 0) return;
  stack->length--;
  *x = stack->array[stack->length].x;
  *y = stack->array[stack->length].y;
}
