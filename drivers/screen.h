#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_MEM 0xa0000
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

int put_pixel_at(int x, int y, unsigned char color);
void put_char_at(int x, int y, unsigned char color, char c);
void clear_screen(unsigned char color);

#endif
