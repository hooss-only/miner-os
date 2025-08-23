#ifndef LOGGER_H
#define LOGGER_H

#include "../drivers/font.h"
#include "../drivers/screen.h"

typedef enum {
  LOG_NORMAL,
  LOG_WARNING,
  LOG_ERROR
} log_level_t;

#define LOG_TEXT_SIZE SCREEN_WIDTH / FONT_WIDTH
typedef struct {
  char text[LOG_TEXT_SIZE];
  log_level_t level;
} log_t;

void append_log(char* text, log_level_t level);
void toggle_log();
void draw_log();
void init_logger();

#endif
