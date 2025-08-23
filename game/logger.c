#include "logger.h"
#include "../libc/memory.h"

#define LOG_MAX_LINES SCREEN_HEIGHT / (FONT_HEIGHT + 2)
log_t logs[LOG_MAX_LINES] = { 0 };
char visible = 0;

void scroll() {
  for (int i=LOG_MAX_LINES-1; i>0; i--) {
    logs[i].level = logs[i-1].level;
    memory_copy(logs[i].text, logs[i-1].text, LOG_TEXT_SIZE);
  }
}

void append_log(char* text, log_level_t level) {
  scroll();
  
  logs[0].level = level;
  memory_copy(logs[0].text, text, LOG_TEXT_SIZE);
}

void toggle_log() {
  visible = !visible;
}

void draw_log() {
  if (!visible) return;

  clear_screen(0);

  char color;
  log_t* log;
  for (int i=0; i<LOG_MAX_LINES; i++) {
    log = &logs[i];
    switch (log->level) {
      case LOG_NORMAL:  color = WHITE; break;
      case LOG_WARNING: color = LIGHT_YELLOW; break;
      case LOG_ERROR:   color = RED; break;
    }
    put_string_at(0, i* (FONT_HEIGHT+2), color, logs[i].text);
  }
}

void init_logger() {
  for (int i=0; i<LOG_MAX_LINES; i++) {
    logs[i].level = LOG_NORMAL;
    logs[i].text[0] = 0;
  }

  visible = 0;
}
