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
    TUI_Engine::clrscr(TUI_Engine::BLUE);
    TUI_Engine::disable_cursor();

    TUI_Engine::draw_window(5, 5, 20, 20, TUI_Engine::YELLOW, TUI_Engine::BLUE, "TESTWINDOW");

    return 0;
}