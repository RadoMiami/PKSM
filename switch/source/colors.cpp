/*
 *   This file is part of Checkpoint
 *   Copyright (C) 2017-2021 Bernardo Giordano, FlagBrew
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
  *         reasonable ways as different from the original version.
 */

#include "colors.hpp"

//Wondering why c4 is used as the background color? I needed to change a background color fast and c4 wasn't being used for anything.
//Also, it's an unintended joke now that the screen gets blown up at the start of every frame now. Maybe we should invest in a defuse kit.
static const struct Theme darkTheme = {
    COLOR_BLACK,        // c0
    COLOR_GREY_BG,      // c1
    COLOR_GREY_DARKER,  // c2
    COLOR_GREY_DARK,    // c3
    COLOR_GREY_BGLIGHT, // c4
    COLOR_GREY_LIGHT,   // c5
    COLOR_WHITE,        // c6
    COLOR_HIGHBLUE,     // c7
    COLOR_GOLD          // c8
};

static const struct Theme lightTheme = {
    COLOR_WHITE,          // c0
    COLOR_GREY_LIGHTEST,  // c1
    COLOR_GREY_LIGHTER,   // c2
    COLOR_GREY_LIGHT,     // c3
    COLOR_GREY_LIGHTERER, // c4
    COLOR_GREY_DARKER,    // c5
    COLOR_BLACK,          // c6
    COLOR_HIGHMINT,       // c7
    COLOR_DARKGOLD,       // c8
};

static const struct Theme pksmTheme = {
    COLOR_BLACK,           // c0
    COLOR_PKSM_DARKBLUE,   // c1
    COLOR_PKSM_DARKERBLUE, // c2
    COLOR_PKSM_BLUE,       // c3
    COLOR_PKSM_BLUE,       // c4
    COLOR_GREY_LIGHTEST,   // c5
    COLOR_WHITE,           // c6
    COLOR_HIGHBLUE,        // c7
    COLOR_GOLD             // c8
};

static struct Theme currentTheme = darkTheme;

void theme(int t)
{
    switch (t) {
        case 2:
            currentTheme = pksmTheme;
            break;
        case 1:
            currentTheme = lightTheme;
            break;
        default:
            currentTheme = darkTheme;
            break;
    }
}

struct Theme theme(void)
{
    return currentTheme;
}