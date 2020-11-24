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

    TUI_Engine::Window window1;
    window1.set_position(20, 10);
    window1.set_size(30, 10);
    window1.set_window_and_title_colors(TUI_Engine::YELLOW, TUI_Engine::BLUE);
    window1.set_title("SETTINGS");
    window1.set_title_type(true);
    window1.set_shadow(true);
    window1.draw();

    TUI_Engine::Window banner;
    banner.set_position(10, 3);
    banner.set_size(60, 3);
    banner.set_window_and_title_colors(TUI_Engine::YELLOW, TUI_Engine::BLUE);
    banner.set_title("SETTINGS");
    banner.set_title_type(false);
    banner.set_shadow(false);
    banner.draw();

    return 0;
}