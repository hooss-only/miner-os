#ifndef QUEUE_H
#define QUEUE_H

#define MAX_QUEUE_LENGTH 600
typedef struct {
  int x;
  int y;
} point_t;

typedef struct {
  int length;
  point_t array[MAX_QUEUE_LENGTH];
} queue_point_t;

void queue_point_init(queue_point_t* queue);
void queue_point_push(queue_point_t* queue, int x, int y);
void queue_point_pop(queue_point_t* queue, int* x, int* y);

#endif
