#include "engine.h"

#include <stdint.h>
#include <dos.h>
#include <stdio>
#include <conio.h>
#include <cstring>

using namespace std;

void TUI_Engine::set_color_textmode()
{
    _asm {
        mov ah, 0x0
        mov al, 0x3 // 80x25 mode
        int 0x10
    }
}

void TUI_Engine::clrscr(uint8_t fgcolor, uint8_t bgcolor, char fillchar)
{
    uint16_t far *video = (uint16_t far *)0xB8000000L;
    uint16_t attrib = (bgcolor << 4) | (fgcolor & 0x0F);
    for (unsigned int i = 0; i < 80 * 25; i++)
    {
        *video++ = fillchar | (attrib << 8);
    }
}

void TUI_Engine::disable_blink()
{
    _asm {
        mov dx, 0x3DA
        in al, dx
        mov dx, 0x3C0
        mov al, 0x30
        out dx, al
        inc dx
        in al, dx
        and al, 0xF7
        dec dx
        out dx, al
    }
}

void TUI_Engine::enable_cursor()
{
    outp(0x3D4, 0x0A);
    outp(0x3D5, (inp(0x3D5) & 0xC0) | 14);

    outp(0x3D4, 0x0B);
    outp(0x3D5, (inp(0x3D5) & 0xE0) | 15);
}

void TUI_Engine::disable_cursor()
{
    outp(0x3D4, 0x0A);
    outp(0x3D5, 0x20);
}

void TUI_Engine::update_cursor(unsigned int x, unsigned int y)
{
    uint16_t pos = y * 80 + x;

    outp(0x3D4, 0x0F);
    outp(0x3D5, (uint8_t)(pos & 0xFF));
    outp(0x3D4, 0x0E);
    outp(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void TUI_Engine::putchar(unsigned int x, unsigned int y, uint8_t color, uint8_t bgcolor, char ch)
{
    uint16_t far *video = (uint16_t far *)0xB8000000L;
    video += (y * 80 + x);
    uint16_t attrib = (bgcolor << 4) | (color & 0x0F);
    *video = ch | (attrib << 8);
}

void TUI_Engine::putchars(unsigned int x, unsigned int y, uint8_t color, uint8_t bgcolor, char ch, unsigned int count)
{
    uint16_t far *video = (uint16_t far *)0xB8000000L;
    video += (y * 80 + x);
    uint16_t attrib = (bgcolor << 4) | (color & 0x0F);

    for (unsigned int i = 0; i < count; i++)
    {
        *video++ = ch | (attrib << 8);
    }
}

void TUI_Engine::print_string(unsigned int x, unsigned int y, uint8_t color, uint8_t bgcolor, const char *s)
{
    uint16_t far *video = (uint16_t far *)0xB8000000L;
    video += (y * 80 + x);
    uint16_t attrib = (bgcolor << 4) | (color & 0x0F);
    char c = *s++;
    while (c != 0x0)
    {
        *video++ = c | (attrib << 8);
        c = *s++;
    }
}

void TUI_Engine::change_attrs(unsigned int x, unsigned int y, uint8_t fgcolor, uint8_t bgcolor)
{
    uint8_t far *video = (uint8_t far *)0xB8000000L;
    video += ((y * 80 + x) << 1) + 1;
    uint8_t attrib = (bgcolor << 4) | (fgcolor & 0x0F);
    *video = attrib;
}

void TUI_Engine::draw_window_1(unsigned int x, unsigned int y, unsigned int width, unsigned int height,
                               uint8_t color, uint8_t bgcolor, uint8_t fillchar,
                               const char *title, bool thick, bool shadow)
{
    // draw corners
    TUI_Engine::putchar(x, y, color, bgcolor, thick ? 201 : 218);
    TUI_Engine::putchar(x + width - 1, y, color, bgcolor, thick ? 187 : 191);
    TUI_Engine::putchar(x, y + height - 1, color, bgcolor, thick ? 200 : 192);
    TUI_Engine::putchar(x + width - 1, y + height - 1, color, bgcolor, thick ? 188 : 217);

    // draw horizontal lines
    TUI_Engine::putchars(x + 1, y, color, bgcolor, thick ? 205 : 196, width - 2);
    TUI_Engine::putchars(x + 1, y + height - 1, color, bgcolor, thick ? 205 : 196, width - 2);

    // draw vertica lines
    for (unsigned i = 0; i < height - 2; i++)
    {
        TUI_Engine::putchar(x, y + 1 + i, color, bgcolor, thick ? 186 : 179);
        TUI_Engine::putchar(x + width - 1, y + 1 + i, color, bgcolor, thick ? 186 : 179);
    }

    // fill with fillchar
    for (unsigned i = 0; i < height - 2; i++)
    {
        TUI_Engine::putchars(x + 1, y + i + 1, color, bgcolor, fillchar, width - 2);
    }

    // write title
    size_t title_length = strlen(title);
    if (title_length > 0)
    {
        unsigned int title_x = x + (unsigned int)((width - title_length) / 2);
        TUI_Engine::print_string(title_x, y, color, bgcolor, title);
    }

    // draw shadow
    if (shadow)
    {
        for (unsigned int i = 0; i < width; i++)
        {
            TUI_Engine::change_attrs(x + 1 + i, y + height, TUI_Engine::DARKGREY, TUI_Engine::BLACK);
        }
        for (unsigned int i = 0; i < height; i++)
        {
            TUI_Engine::change_attrs(x + width, y + 1 + i, TUI_Engine::DARKGREY, TUI_Engine::BLACK);
            TUI_Engine::change_attrs(x + width + 1, y + 1 + i, TUI_Engine::DARKGREY, TUI_Engine::BLACK);
        }
    }
}

void TUI_Engine::draw_window_2(unsigned int x, unsigned int y, unsigned int width, unsigned int height,
                               uint8_t color, uint8_t bgcolor, uint8_t fillchar,
                               const char *title, bool thick, bool thickheadline, bool shadow)
{
    // basis for window 2 is window 1
    TUI_Engine::draw_window_1(x, y, width, height, color, bgcolor, fillchar, "", thick, shadow);

    // draw title in 2nd row
    size_t title_length = strlen(title);
    unsigned int title_x = x + (unsigned int)((width - title_length) / 2);
    TUI_Engine::print_string(title_x, y + 1, color, bgcolor, title);

    // draw line under title in 3rd row
    TUI_Engine::putchars(x + 1, y + 2, color, bgcolor, thickheadline ? 205 : 196, width - 2);

    // fix corner chars
    if (thick == thickheadline)
    {
        // on my codepage, it only works for arguments true, true or false, false
        TUI_Engine::putchar(x, y + 2, color, bgcolor,
                            thick ? (thickheadline ? 204 : 199) : (thickheadline ? 198 : 195));
        TUI_Engine::putchar(x + width - 1, y + 2, color, bgcolor,
                            thick ? (thickheadline ? 185 : 182) : (thickheadline ? 181 : 180));
    }
}