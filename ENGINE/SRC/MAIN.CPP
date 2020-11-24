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

    TUI_Engine::draw_window_1(20, 10, 30, 10, TUI_Engine::YELLOW, TUI_Engine::BLUE,
                              TUI_Engine::ONLYBACKGROUND, "SETTINGS", true, false);
    TUI_Engine::draw_window_2(2, 2, 20, 15, TUI_Engine::YELLOW, TUI_Engine::BLUE,
                              TUI_Engine::ONLYBACKGROUND, "SETTINGS", false, true, true);

    return 0;
}