#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_MEM_TEXT 0xb8000
#define TEXT_WIDTH 80
#define TEXT_HEIGHT 25

#define VIDEO_MEM 0xa0000
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

void clear_text();

#endif
