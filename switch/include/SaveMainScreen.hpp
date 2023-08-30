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

#ifndef SAVEMAINSCREEN_HPP
#define SAVEMAINSCREEN_HPP

#include "ErrorOverlay.hpp"
#include "InfoOverlay.hpp"
#include "Screen.hpp"
#include "YesNoOverlay.hpp"
#include "MainScreen.hpp"
#include "clickable.hpp"
#include "hid.hpp"
#include "io.hpp"
#include "main.hpp"
#include "pksmbridge.hpp"
#include "scrollable.hpp"
#include <tuple>

class Clickable;
class Scrollable;

class SaveMainScreen : public Screen {
public:
    SaveMainScreen(Title title, std::string inputFilePath);
    void draw(void) const override;
    void update(const InputState& input) override;

protected:
    void handleEvents(const InputState& input);

private:
    int selectionTimer;
    bool wantInstructions;
    std::unique_ptr<Clickable> buttonStorage, buttonEditor, buttonEvents, buttonScripts, buttonBag, buttonSettings;
    char ver[8];
};

#endif
