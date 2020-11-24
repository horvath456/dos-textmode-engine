#include "engine.h"

#include <stdint.h>
#include <dos.h>
#include <stdio>
#include <conio.h>
#include <cstring>
#include <string>

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

TUI_Engine::Window::Window()
{
    x = 0;
    y = 0;
    width = 78;
    height = 24;
    fgcolor = LIGHTCYAN;
    bgcolor = BLUE;
    title_fgcolor = LIGHTCYAN;
    title_bgcolor = BLUE;
    fillchar = ONLYBACKGROUND;
    title = "";
    thick = true;
    shadow = true;
    title_top = true;
    title_centered = true;
    title_thick_underline = false;
}
void TUI_Engine::Window::set_position(unsigned int new_x, unsigned int new_y)
{
    x = new_x;
    y = new_y;
}
void TUI_Engine::Window::set_size(unsigned int new_width, unsigned int new_height)
{
    width = new_width;
    height = new_height;
}
void TUI_Engine::Window::set_window_colors(uint8_t new_fgcolor, uint8_t new_bgcolor)
{
    fgcolor = new_fgcolor;
    bgcolor = new_bgcolor;
}
void TUI_Engine::Window::set_window_and_title_colors(uint8_t new_fgcolor, uint8_t new_bgcolor)
{
    fgcolor = new_fgcolor;
    bgcolor = new_bgcolor;
    title_fgcolor = new_fgcolor;
    title_bgcolor = new_bgcolor;
}
void TUI_Engine::Window::set_fillchar(uint8_t new_fillchar)
{
    fillchar = new_fillchar;
}
void TUI_Engine::Window::set_shadow(bool new_shadow)
{
    shadow = new_shadow;
}
void TUI_Engine::Window::set_thickness(bool new_thick)
{
    thick = new_thick;
}
void TUI_Engine::Window::set_title(const char *new_title)
{
    title = new_title;
}
void TUI_Engine::Window::set_title_type(bool new_title_top, bool new_centered, bool new_thick_underline)
{
    title_top = new_title_top;
    title_thick_underline = new_thick_underline;
    title_centered = new_centered;
}
void TUI_Engine::Window::set_title_colors(uint8_t new_fgcolor, uint8_t new_bgcolor)
{
    title_fgcolor = new_fgcolor;
    title_bgcolor = new_bgcolor;
}
void TUI_Engine::Window::draw()
{
    // draw corners
    TUI_Engine::putchar(x, y, fgcolor, bgcolor, thick ? 201 : 218);
    TUI_Engine::putchar(x + width - 1, y, fgcolor, bgcolor, thick ? 187 : 191);
    TUI_Engine::putchar(x, y + height - 1, fgcolor, bgcolor, thick ? 200 : 192);
    TUI_Engine::putchar(x + width - 1, y + height - 1, fgcolor, bgcolor, thick ? 188 : 217);

    // draw horizontal lines
    TUI_Engine::putchars(x + 1, y, fgcolor, bgcolor, thick ? 205 : 196, width - 2);
    TUI_Engine::putchars(x + 1, y + height - 1, fgcolor, bgcolor, thick ? 205 : 196, width - 2);

    // draw vertical lines
    for (unsigned i = 0; i < height - 2; i++)
    {
        TUI_Engine::putchar(x, y + 1 + i, fgcolor, bgcolor, thick ? 186 : 179);
        TUI_Engine::putchar(x + width - 1, y + 1 + i, fgcolor, bgcolor, thick ? 186 : 179);
    }

    // fill with fillchar
    for (unsigned i = 0; i < height - 2; i++)
    {
        TUI_Engine::putchars(x + 1, y + i + 1, fgcolor, bgcolor, fillchar, width - 2);
    }

    // draw title
    size_t title_length = title.size();
    if (title_length > 0)
    {
        unsigned int title_x;
        if (title_centered)
        {
            title_x = x + (unsigned int)((width - title_length) / 2);
        }
        else
        {
            title_x = x + 2;
        }

        if (title_top) // write title in top row
        {
            TUI_Engine::print_string(title_x, y, title_fgcolor, title_bgcolor, title.c_str());
        }
        else // draw title in 2nd row
        {
            TUI_Engine::putchars(x + 1, y + 1, title_fgcolor, title_bgcolor, TUI_Engine::ONLYBACKGROUND, width - 2);
            TUI_Engine::print_string(title_x, y + 1, title_fgcolor, title_bgcolor, title.c_str());

            if (height > 3)
            {
                // draw line under title in 3rd row if the window height is more than 3
                TUI_Engine::putchars(x + 1, y + 2, fgcolor, bgcolor, title_thick_underline ? 205 : 196, width - 2);

                // fix corner chars
                if (thick == title_thick_underline)
                {
                    // on my codepage, it only works for arguments true, true or false, false
                    TUI_Engine::putchar(x, y + 2, fgcolor, bgcolor,
                                        thick ? (title_thick_underline ? 204 : 199) : (title_thick_underline ? 198 : 195));
                    TUI_Engine::putchar(x + width - 1, y + 2, fgcolor, bgcolor,
                                        thick ? (title_thick_underline ? 185 : 182) : (title_thick_underline ? 181 : 180));
                }
            }
        }
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
