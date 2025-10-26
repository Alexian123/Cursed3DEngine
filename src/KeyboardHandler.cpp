#include "KeyboardHandler.h"

#include <ncursesw/ncurses.h>
#include <ctype.h>

using namespace Cursed3DEngine;

KeyboardHandler::KeyboardHandler()
{
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    timeout(0);

    mousemask(ALL_MOUSE_EVENTS, NULL);
    mouseinterval(0);
}

KeyboardHandler::~KeyboardHandler() {}

PressedKey KeyboardHandler::getPressedKey() const
{
    int rawKeyValue = getch();
    if (rawKeyValue == ERR) {
        return KBD_NONE;
    }

    PressedKey key;
    switch (toupper(rawKeyValue)) {
        case 'Q':
            key = KBD_Q;
            break;

        case 'W':
            key = KBD_W;
            break;

        case 'S':
            key = KBD_S;
            break;

        case 'A':
            key = KBD_A;
            break;

        case 'D':
            key = KBD_D;
            break;

        default:
            key = KBD_NONE;
            break;
    }
    return key;
}