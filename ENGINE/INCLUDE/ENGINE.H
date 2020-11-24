/* engine.h */

#ifndef __engine_h__
#define __engine_h__

#include <stdint.h>
#include <string>

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

    enum CLRSCR_FILLCHAR
    {
        ONLYBACKGROUND = 0x20, // SPACE CHAR
        ONLYFOREGROUND = 219,  // FILLED RECT
        DOT_PATTERN_1 = 176,
        DOT_PATTERN_2 = 177,
        DOT_PATTERN_3 = 178
    };

    void set_color_textmode();
    void disable_blink();
    void clrscr(uint8_t fgcolor = TUI_Engine::LIGHTGREY, uint8_t bgcolor = TUI_Engine::BLACK,
                char fillchar = TUI_Engine::ONLYBACKGROUND);

    void enable_cursor();
    void disable_cursor();
    void update_cursor(unsigned int x, unsigned int y);

    void putchar(unsigned int x, unsigned int y, uint8_t color, uint8_t bgcolor, char ch);
    void putchars(unsigned int x, unsigned int y, uint8_t color, uint8_t bgcolor, char ch, unsigned int count);
    void print_string(unsigned int x, unsigned int y, uint8_t color, uint8_t bgcolor, const char *s);
    void change_attrs(unsigned int x, unsigned int y, uint8_t fgcolor, uint8_t bgcolor);

    class Window
    {
    public:
        Window();
        void set_position(unsigned int x, unsigned int y);
        void set_size(unsigned int width, unsigned int height);
        void set_colors(uint8_t fgcolor, uint8_t bgcolor);
        void set_fillchar(uint8_t fillchar);
        void set_shadow(bool shadow);
        void set_thickness(bool thick);
        void set_window_title(const char *title);
        void set_window_title_type(bool title_top, bool thick_underline);
        void draw();

    private:
        unsigned int x;
        unsigned int y;
        unsigned int width;
        unsigned int height;
        uint8_t fgcolor;
        uint8_t bgcolor;
        uint8_t fillchar;
        std::string title;
        bool thick;
        bool shadow;
        bool title_top;
        bool thick_underline;
    };
}; // namespace TUI_Engine

#endif