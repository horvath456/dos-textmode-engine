/* engine.h */

#ifndef __engine_h__
#define __engine_h__

#include <stdint.h>

namespace TUI_Engine
{
    enum VIDEO_COLOR
    {
        BLACK = 0x0,
        BLUE = 0x1,
        GREEN = 0x2,
        CYAN = 0x3,
        RED = 0x4,
        MAGENTA = 0x5,
        BROWN = 0x6,
        LIGHTGREY = 0x7,
        DARKGREY = 0x8,
        LIGHTBLUE = 0x9,
        LIGHTGREEN = 0xA,
        LIGHTCYAN = 0xB,
        LIGHTRED = 0xC,
        LIGHTMAGENTA = 0xD,
        YELLOW = 0xE,
        WHITE = 0xF
    };

    void set_color_textmode();
    void clrscr(uint8_t bgcolor);
    void enable_cursor();
    void disable_cursor();
    void update_cursor(unsigned int x, unsigned int y);
    void putchar(unsigned int x, unsigned int y, uint8_t color, uint8_t bgcolor, char ch);
    void print_string(unsigned int x, unsigned int y, uint8_t color, uint8_t bgcolor, const char *s);
    void disable_blink();
};

#endif