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

#include "SaveMainScreen.hpp"
#include "MainScreen.hpp"
#include "main.hpp"

Title currentTitle;

SaveMainScreen::SaveMainScreen(Title title)
{
    wantInstructions = false;
    currentTitle = title;
    selectionTimer   = 0;
    sprintf(ver, "v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);
    buttonStorage  = std::make_unique<Clickable>(8, 65, 400, 70, theme().c2, theme().c6, "Storage", false);
    buttonEditor = std::make_unique<Clickable>(8, 165, 400, 70, theme().c2, theme().c6, "Editor", false);
    buttonEvents  = std::make_unique<Clickable>(8, 265, 400, 70, theme().c2, theme().c6, "Events", false);
    buttonScripts = std::make_unique<Clickable>(8, 365, 400, 70, theme().c2, theme().c6, "Scripts", false);
    buttonBag  = std::make_unique<Clickable>(8, 465, 400, 70, theme().c2, theme().c6, "Bag", false);
    buttonSettings = std::make_unique<Clickable>(8, 565, 400, 70, theme().c2, theme().c6, "Settings", false);
    buttonStorage->canChangeColorWhenSelected(true);
    buttonEditor->canChangeColorWhenSelected(true);
    buttonEvents->canChangeColorWhenSelected(true);
    buttonScripts->canChangeColorWhenSelected(true);
    buttonBag->canChangeColorWhenSelected(true);
    buttonSettings->canChangeColorWhenSelected(true);
}

void SaveMainScreen::draw() const
{
    //Replace this with the animated background! Wait, how do I do that?
    SDLH_ClearScreen(theme().c4);

    SDLH_DrawRect(0, 662, 1280, 720, theme().c1);

    u32 ver_w, ver_h, checkpoint_h, checkpoint_w, inst_w, inst_h;
    SDLH_GetTextDimensions(20, ver, &ver_w, &ver_h);
    SDLH_GetTextDimensions(24, "almostpksm", &checkpoint_w, &checkpoint_h);
    SDLH_GetTextDimensions(24, "\ue046 Instructions", &inst_w, &inst_h);

    drawOutline(8, 65, 400, 70, 4, theme().c3);
    drawOutline(8, 165, 400, 70, 4, theme().c3);
    drawOutline(8, 265, 400, 70, 4, theme().c3);
    drawOutline(8, 365, 400, 70, 4, theme().c3);
    drawOutline(8, 465, 400, 70, 4, theme().c3);
    drawOutline(8, 565, 400, 70, 4, theme().c3);
    buttonStorage->draw(30, COLOR_NULL);
    buttonEditor->draw(30, COLOR_NULL);
    buttonEvents->draw(30, COLOR_NULL);
    buttonScripts->draw(30, COLOR_NULL);
    buttonBag->draw(30, COLOR_NULL);
    buttonSettings->draw(30, COLOR_NULL);

    u32 title_w, title_h;
    auto displayName = StringUtils::removeAccents(currentTitle.name());
    SDLH_GetTextDimensions(28, displayName.c_str(), &title_w, &title_h);
        if (title_w >= 534) {
        displayName = displayName.substr(0, 24) + "...";
        SDLH_GetTextDimensions(28, displayName.c_str(), &title_w, &title_h);
    }

    if (currentTitle.icon() != NULL) {
            drawOutline(1010, 16 + title_h, 256, 256, 4, theme().c3);
            SDLH_DrawImage(currentTitle.icon(), 1010, 16 + title_h);
    }

    SDLH_DrawRect(0, 0, 1280, 12 + title_h, theme().c1);
    SDLH_DrawText(28, 1280 - 16 - title_w, 8, theme().c5, displayName.c_str());

    if (wantInstructions && currentOverlay == nullptr) {
        SDLH_DrawRect(0, 0, 1280, 720, FC_MakeColor(theme().c0.r, theme().c0.g, theme().c0.b, 170));
        SDLH_DrawText(24, 58, 69, theme().c6, "\ue058 Tap to select option");
        SDLH_DrawText(24, 100, 270, theme().c6, "I haven't put in button controls yet, oops");
    }

    SDLH_DrawRect(0, 2, checkpoint_w + ver_w + 2 * 16 + 20, 40, theme().c1);
    SDLH_DrawText(26, 16, (40 - checkpoint_h) / 2 + 4, theme().c6, "almostpksm");
    SDLH_DrawText(20, 34 + checkpoint_w, (40 - checkpoint_h) / 2 + checkpoint_h - ver_h + 4, theme().c6, ver);
    SDLH_DrawText(24, 16, 672 + (40 - checkpoint_h) / 2 + checkpoint_h - inst_h, theme().c6, "\ue046 Instructions");
}

void SaveMainScreen::update(const InputState& input)
{
    handleEvents(input);
}

void SaveMainScreen::handleEvents(const InputState& input)
{
    const u64 kheld = input.kHeld;
    const u64 kdown = input.kDown;

    wantInstructions = (kheld & HidNpadButton_Minus);

    // Handle pressing A
    // Open up menu depending on button
    if (kdown & HidNpadButton_A) {

    }

    // Handle pressing B
    // Go back to main screen
    if (kdown & HidNpadButton_B) {
        changeScreen(std::make_unique<MainScreen>(input));
    }
}