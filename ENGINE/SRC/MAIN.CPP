#include <iostream>
#include <stdint.h>
#include <dos.h>
#include <stdio>
#include <conio.h>

#include "engine.h"

using namespace std;

int main()
{
    TUI_Engine::set_color_textmode();
    TUI_Engine::disable_blink();
    TUI_Engine::clrscr(TUI_Engine::YELLOW);
    TUI_Engine::putchar(5, 5, TUI_Engine::WHITE, TUI_Engine::RED, '-');
    TUI_Engine::print_string(10, 2, TUI_Engine::RED, TUI_Engine::WHITE, "Hallo, das ist ein TEST ----___");
    TUI_Engine::disable_cursor();
    TUI_Engine::enable_cursor();
    TUI_Engine::update_cursor(10, 10);

    return 0;
}