#include "queue.h"

void queue_point_init(queue_point_t* queue) {
  queue->length = 0;
}

void queue_point_push(queue_point_t* queue, int x, int y) {
  if (queue->length == MAX_QUEUE_LENGTH) return; 
  queue->length++;
  queue->array[queue->length - 1].x = x;
  queue->array[queue->length - 1].y = y;
}

void queue_point_pop(queue_point_t* queue, int* x, int* y) {
  if (queue->length == 0) return;
  queue->length--;
  *x = queue->array[queue->length].x;
  *y = queue->array[queue->length].y;
}
