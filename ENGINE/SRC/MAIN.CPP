#include <iostream>
#include <stdint.h>
#include <dos.h>
#include <stdio>
#include <conio.h>

using namespace std;

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

void set_color_textmode()
{
    _asm {
        mov ah, 0x0
        mov al, 0x3 // 80x25 mode
        int 0x10
    }
}

void clrscr(uint8_t bgcolor)
{
    uint16_t far *video = (uint16_t far *)0xB8000000L;
    for (unsigned int i = 0; i < 80 * 25; i++)
    {
        *video++ = 0x20 | (bgcolor << 12);
    }
}

void enable_cursor()
{
    outp(0x3D4, 0x0A);
    outp(0x3D5, (inp(0x3D5) & 0xC0) | 14);

    outp(0x3D4, 0x0B);
    outp(0x3D5, (inp(0x3D5) & 0xE0) | 15);
}

void disable_cursor()
{
    outp(0x3D4, 0x0A);
    outp(0x3D5, 0x20);
}

void update_cursor(unsigned int x, unsigned int y)
{
    uint16_t pos = y * 80 + x;

    outp(0x3D4, 0x0F);
    outp(0x3D5, (uint8_t)(pos & 0xFF));
    outp(0x3D4, 0x0E);
    outp(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

inline void putchar(unsigned int x, unsigned int y, uint8_t color, uint8_t bgcolor, char ch)
{
    uint16_t far *video = (uint16_t far *)0xB8000000L;
    video += (y * 80 + x);
    uint16_t attrib = (bgcolor << 4) | (color & 0x0F);
    *video = ch | (attrib << 8);
}

void print_string(unsigned int x, unsigned int y, uint8_t color, uint8_t bgcolor, const char *s)
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

void disable_blink()
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

int main()
{
    set_color_textmode();
    disable_blink();
    clrscr(YELLOW);
    putchar(5, 5, WHITE, RED, '-');
    print_string(10, 2, RED, WHITE, "Hallo, das ist ein TEST ----___");
    disable_cursor();
    enable_cursor();
    update_cursor(10, 10);

    return 0;
}