#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_MEM 0xa0000
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

#define LIGHT_GRAY 7
#define LIGHT_YELLOW 14
#define WHITE 15

int put_pixel_at(int x, int y, unsigned char color);
void put_char_at(int x, int y, unsigned char color, char c);
void put_string_at(int x, int y, unsigned char color, char* str);
void clear_screen(unsigned char color);

#endif
