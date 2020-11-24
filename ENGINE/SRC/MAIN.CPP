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
    TUI_Engine::clrscr(TUI_Engine::CYAN, TUI_Engine::BLUE, TUI_Engine::DOT_PATTERN_1);
    TUI_Engine::disable_cursor();

    TUI_Engine::draw_window(2, 2, 76, 18, TUI_Engine::YELLOW, TUI_Engine::BLUE, "SETTINGS");

    return 0;
}