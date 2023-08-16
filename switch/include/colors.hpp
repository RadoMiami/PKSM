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
 *   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
 *       * Requiring preservation of specified reasonable legal notices or
 *         author attributions in that material or in the Appropriate Legal
 *         Notices displayed by works containing it.
 *       * Prohibiting misrepresentation of the origin of that material,
 *         or requiring that modified versions of such material be marked in
 *         reasonable ways as different from the original version.
 */

#ifndef COLORS_HPP
#define COLORS_HPP

#include "SDLHelper.hpp"

inline const SDL_Color COLOR_WHITE     = FC_MakeColor(255, 255, 255, 255);
inline const SDL_Color COLOR_BLACK     = FC_MakeColor(0, 0, 0, 255);
inline const SDL_Color COLOR_BLUE      = FC_MakeColor(29, 50, 243, 255);
inline const SDL_Color COLOR_GREEN     = FC_MakeColor(0, 254, 197, 255);
inline const SDL_Color COLOR_RED       = FC_MakeColor(255, 81, 48, 255);
inline const SDL_Color COLOR_GOLD      = FC_MakeColor(255, 215, 0, 255);
inline const SDL_Color COLOR_OVERLAY   = FC_MakeColor(0, 0, 0, 190);
inline const SDL_Color COLOR_WHITEMASK = FC_MakeColor(255, 255, 255, 80);
inline const SDL_Color COLOR_NULL      = FC_MakeColor(0, 0, 0, 0);

inline const SDL_Color COLOR_GREY_BG     = FC_MakeColor(51, 51, 51, 255);
inline const SDL_Color COLOR_GREY_DARKER = FC_MakeColor(70, 70, 70, 255);
inline const SDL_Color COLOR_GREY_DARK   = FC_MakeColor(79, 79, 79, 255);
inline const SDL_Color COLOR_GREY_MEDIUM = FC_MakeColor(94, 94, 94, 255);
inline const SDL_Color COLOR_GREY_LIGHT  = FC_MakeColor(138, 138, 138, 255);
inline const SDL_Color COLOR_HIGHBLUE    = FC_MakeColor(48, 65, 106, 255);
inline const SDL_Color COLOR_WATERMARK   = FC_MakeColor(255, 0, 0, 140);

inline const SDL_Color COLOR_GREY_LIGHTEST   = FC_MakeColor(222, 222, 222, 255);
inline const SDL_Color COLOR_GREY_LIGHTER    = FC_MakeColor(196, 196, 196, 255);
inline const SDL_Color COLOR_HIGHMINT        = FC_MakeColor(83, 150, 252, 255);
inline const SDL_Color COLOR_DARKGOLD        = FC_MakeColor(255, 173, 0, 255);
inline const SDL_Color COLOR_PKSM_DARKBLUE   = FC_MakeColor(15, 22, 131, 255);
inline const SDL_Color COLOR_PKSM_BLUE       = FC_MakeColor(26, 35, 126, 255);
inline const SDL_Color COLOR_PKSM_LIGHTBLUE  = FC_MakeColor(55, 89, 187, 255);
inline const SDL_Color COLOR_PKSM_DARKERBLUE = FC_MakeColor(15, 22, 89, 255);
inline const SDL_Color COLOR_GREY_BGLIGHT    = FC_MakeColor(61, 61, 61, 255);
inline const SDL_Color COLOR_GREY_LIGHTERER  = FC_MakeColor(212, 212, 212, 255);

struct Theme {
    SDL_Color c0;
    SDL_Color c1;
    SDL_Color c2;
    SDL_Color c3;
    SDL_Color c4;
    SDL_Color c5;
    SDL_Color c6;
    SDL_Color c7;
    SDL_Color c8;
};

#define THEME_DEFAULT 0

void theme(int t);
struct Theme theme(void);

#endif