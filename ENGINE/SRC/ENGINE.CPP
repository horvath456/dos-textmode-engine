#include "engine.h"

#include <stdint.h>
#include <dos.h>
#include <stdio>
#include <conio.h>

using namespace std;

void TUI_Engine::set_color_textmode()
{
    _asm {
        mov ah, 0x0
        mov al, 0x3 // 80x25 mode
        int 0x10
    }
}

void TUI_Engine::clrscr(uint8_t bgcolor)
{
    uint16_t far *video = (uint16_t far *)0xB8000000L;
    for (unsigned int i = 0; i < 80 * 25; i++)
    {
        *video++ = 0x20 | (bgcolor << 12);
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