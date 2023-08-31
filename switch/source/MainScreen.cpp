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

#include "MainScreen.hpp"
#include "SaveMainScreen.hpp"
#include "main.hpp"

//Why is the thing that controls the length of the collumns rowlen?
static constexpr size_t rowlen = 99, collen = 1, rows = 8, SIDEBAR_w = 96, imageSize = 256, iconOffsetX = 8, iconOffsetY = 64, scrollupentry = 2;

MainScreen::MainScreen(const InputState& input) : hid(rowlen * collen, collen, input)
{
    pksmBridge       = false;
    wantInstructions = false;
    selectionTimer   = 0;
    sprintf(ver, "v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);
    backupList    = std::make_unique<Scrollable>(428, 320, 414, 304, rows);
    buttonBackup  = std::make_unique<Clickable>(870, 320, 220, 80, theme().c2, theme().c6, "Send \ue004", true);
    buttonRestore = std::make_unique<Clickable>(870, 420, 220, 80, theme().c2, theme().c6, "Recieve \ue005", true);
    buttonBackup->canChangeColorWhenSelected(true);
    buttonRestore->canChangeColorWhenSelected(true);
}

int MainScreen::selectorX(size_t i) const
{
    return (imageSize * ((i % (rowlen * collen)) % collen) + 4 * (((i % (rowlen * collen)) % collen) + 1)) + iconOffsetX;
}

int MainScreen::selectorY(size_t i) const
{
    return (imageSize * ((i % (rowlen * collen)) / collen) + 4 * (((i % (rowlen * collen)) / collen) + 1)) + iconOffsetY;
}

void MainScreen::draw() const
{
    const size_t entries = hid.maxVisibleEntries();
    const size_t max     = hid.maxEntries(getTitleCount(g_currentUId)) + 1;

    SDLH_ClearScreen(theme().c4);

    // title icons
    for (size_t k = hid.page() * entries; k < hid.page() * entries + max; k++) {
        int selectorx = selectorX(k - std::clamp((int) hid.index() - 1, 0, std::numeric_limits<int>::max()));
        int selectory = selectorY(k - std::clamp((int) hid.index() - 1, 0, std::numeric_limits<int>::max()));
        if (smallIcon(g_currentUId, k) != NULL) {
            SDLH_DrawImageScale(smallIcon(g_currentUId, k), selectorx, selectory, imageSize, imageSize);
        }
    }

    // title selector
    if (getTitleCount(g_currentUId) > 0) {
        const int x = selectorX(hid.index() > 0 ? 1 : 0) + 4 / 2;
        const int y = selectorY(hid.index() > 0 ? 1 : 0) + 4 / 2;
        drawPulsingOutline(x, y, imageSize - 4, imageSize - 4, 4, COLOR_BLUE);
        SDLH_DrawRect(x, y, imageSize - 4, imageSize - 4, COLOR_WHITEMASK);
    }

    if (getTitleCount(g_currentUId) > 0) {
        Title title;
        getTitle(title, g_currentUId, hid.fullIndex());

        backupList->flush();
        std::vector<std::string> dirs = title.saves();

        for (size_t i = 0; i < dirs.size(); i++) {
            backupList->push_back(theme().c2, theme().c6, dirs.at(i), i == backupList->index());
        }

        // draw infos
        u32 title_w, title_h;
        auto displayName = StringUtils::removeAccents(title.name());
        SDLH_GetTextDimensions(28, displayName.c_str(), &title_w, &title_h);

        if (title_w >= 534) {
            displayName = displayName.substr(0, 24) + "...";
            SDLH_GetTextDimensions(28, displayName.c_str(), &title_w, &title_h);
        }

        SDLH_DrawRect(0, 0, 1280, 12 + title_h, theme().c1);
        SDLH_DrawText(28, 1280 - 28 - title_w, 8, theme().c5, displayName.c_str());

        drawOutline(428, 320, 414, 304, 4, theme().c3);
        backupList->draw(g_backupScrollEnabled);

        if (pksmBridge)
        {
            drawOutline(870, 320, 220, 80, 4, theme().c3);
            drawOutline(870, 420, 220, 80, 4, theme().c3);
            buttonBackup->draw(30, COLOR_NULL);
            buttonRestore->draw(30, COLOR_NULL);
        }
    }

    SDLH_DrawRect(0, 662, 1280, 720, theme().c1);

    drawPulsingOutline(
        1280 - SIDEBAR_w + (SIDEBAR_w - USER_ICON_SIZE) / 2, 720 - USER_ICON_SIZE - 30, USER_ICON_SIZE, USER_ICON_SIZE, 2, COLOR_GREEN);
    SDLH_DrawImageScale(
        Account::icon(g_currentUId), 1280 - SIDEBAR_w + (SIDEBAR_w - USER_ICON_SIZE) / 2, 720 - USER_ICON_SIZE - 30, USER_ICON_SIZE, USER_ICON_SIZE);

    u32 username_w, username_h;
    std::string username = Account::shortName(g_currentUId);
    SDLH_GetTextDimensions(13, username.c_str(), &username_w, &username_h);
    SDLH_DrawTextBox(13, 1280 - SIDEBAR_w + (SIDEBAR_w - username_w) / 2, 720 - 28 + (28 - username_h) / 2, theme().c6, SIDEBAR_w, username.c_str());

    u32 ver_w, ver_h, checkpoint_h, checkpoint_w, inst_w, inst_h;
    SDLH_GetTextDimensions(20, ver, &ver_w, &ver_h);
    SDLH_GetTextDimensions(24, "almostpksm", &checkpoint_w, &checkpoint_h);
    SDLH_GetTextDimensions(24, "\ue046 Instructions", &inst_w, &inst_h);

    if (wantInstructions && currentOverlay == nullptr) {
        SDLH_DrawRect(0, 0, 1280, 720, FC_MakeColor(theme().c0.r, theme().c0.g, theme().c0.b, 170));
        SDLH_DrawText(27, 1205, 646, theme().c6, "\ue085\ue086");
        SDLH_DrawText(24, 58, 69, theme().c6, "\ue058 Tap to select title");
        SDLH_DrawText(24, 58, 109, theme().c6, ("\ue026 Sort: " + sortMode()).c_str());
        SDLH_DrawText(24, 100, 270, theme().c6, "\ue006 \ue080 to scroll between titles");
        SDLH_DrawText(24, 100, 300, theme().c6, "\ue000 to enter the selected title");
        SDLH_DrawText(24, 100, 330, theme().c6, "\ue001 to exit the selected title");
        if (gethostid() != INADDR_LOOPBACK) {
            if (g_ftpAvailable && Configuration::getInstance().isFTPEnabled()) {
                SDLH_DrawText(24, 16 * 6 + checkpoint_w + 8 + ver_w + inst_w, 642 + (40 - checkpoint_h) / 2 + checkpoint_h - inst_h, theme().c8,
                    StringUtils::format("FTP server running on %s:50000", getConsoleIP()).c_str());
            }
            SDLH_DrawText(24, 16 * 6 + checkpoint_w + 8 + ver_w + inst_w, 672 + (40 - checkpoint_h) / 2 + checkpoint_h - inst_h, theme().c8,
                StringUtils::format("Configuration server running on %s:8000", getConsoleIP()).c_str());
        }
    }

    SDLH_DrawRect(0, 2, checkpoint_w + ver_w + 2 * 16 + 20, 40, theme().c1);
    SDLH_DrawText(26, 16, (40 - checkpoint_h) / 2 + 4, theme().c6, "almostpksm");
    SDLH_DrawText(20, 34 + checkpoint_w, (40 - checkpoint_h) / 2 + checkpoint_h - ver_h + 4, theme().c6, ver);
    SDLH_DrawText(24, 16, 672 + (40 - checkpoint_h) / 2 + checkpoint_h - inst_h, theme().c6, "\ue046 Settings");

    if (g_isTransferringFile) {
        SDLH_DrawRect(0, 0, 1280, 720, FC_MakeColor(0, 0, 0, 170));

        u32 w, h;
        SDLH_GetTextDimensions(28, g_currentFile.c_str(), &w, &h);
        SDLH_DrawText(28, (1280 - w) / 2, (720 - h) / 2, COLOR_WHITE, g_currentFile.c_str());
    }
}

void MainScreen::update(const InputState& input)
{
    updateSelector(input);
    handleEvents(input);
}

void MainScreen::updateSelector(const InputState& input)
{
    if (!g_backupScrollEnabled) {
        size_t count    = getTitleCount(g_currentUId);
        size_t oldindex = hid.index();
        hid.update(count);

        // loop through every rendered title
        for (u8 row = 0; row < rowlen; row++) {
            for (u8 col = 0; col < collen; col++) {
                u8 index = row * collen + col;
                if (index > hid.maxEntries(count))
                    break;

                u32 x = selectorX(index);
                u32 y = selectorY(index);
                if (input.touch.count > 0 && input.touch.touches[0].x >= x && input.touch.touches[0].x <= x + imageSize && input.touch.touches[0].y >= y &&
                    input.touch.touches[0].y <= y + imageSize) {
                    hid.index(index);
                }
            }
        }

        backupList->resetIndex();
        if (hid.index() != oldindex) {
            setPKSMBridgeFlag(false);
        }
    }
    else {
        backupList->updateSelection();
    }
}

void MainScreen::handleEvents(const InputState& input)
{
    const u64 kheld = input.kHeld;
    const u64 kdown = input.kDown;

    wantInstructions = (kheld & HidNpadButton_Minus);

    if (kdown & HidNpadButton_ZL || kdown & HidNpadButton_ZR) {
        while ((g_currentUId = Account::selectAccount()) == 0)
            ;
        this->index(TITLES, 0);
        this->index(CELLS, 0);
        setPKSMBridgeFlag(false);
    }
    // handle PKSM bridge
    Title title;
    getTitle(title, g_currentUId, this->index(TITLES));
    if (isPKSMBridgeTitle(title.id())) {
        setPKSMBridgeFlag(true);
        updateButtons();
    }

    // handle touchscreen
    if (!g_backupScrollEnabled && input.touch.count > 0 && input.touch.touches[0].x >= 1200 && input.touch.touches[0].x <= 1200 + USER_ICON_SIZE &&
        input.touch.touches[0].y >= 626 && input.touch.touches[0].y <= 626 + USER_ICON_SIZE) {
        while ((g_currentUId = Account::selectAccount()) == 0)
            ;
        this->index(TITLES, 0);
        this->index(CELLS, 0);
        setPKSMBridgeFlag(false);
    }

    // Handle pressing A
    // Copy save to sdmc:/switch/PKSM/temp
    if (kdown & HidNpadButton_A) {
        if (g_backupScrollEnabled) {
            currentOverlay = std::make_shared<YesNoOverlay>(
                *this, "Open this save?",
                [this]() {
                    if (io::directoryExists(TEMP_PATH + "/")) { io::deleteFolderRecursively(TEMP_PATH + "/");}
                    io::createDirectory(TEMP_PATH + "/");
                    Title title;
                    getTitle(title, g_currentUId, hid.fullIndex());
                    if (0 == this->index(CELLS)) {
                        auto result = io::backup(this->index(TITLES), g_currentUId, this->index(CELLS));
                        if (std::get<0>(result)) {
                            //I would have moved this code below so I could still show an information pop up, but
                            //the non-euclidean code apparently forbids it. Either that or I'm dumb. Probably a bit of both.
                            this->index(CELLS, 0);
                            g_backupScrollEnabled = false;
                            entryType(TITLES);
                            //Probably going to have to change this when support for non-Switch games are added.
                            if (io::fileExists(TEMP_PATH + "/main")) {
                                changeScreen(std::make_unique<SaveMainScreen>(title, TEMP_PATH + "/main"));
                            } else {
                                currentOverlay = std::make_shared<ErrorOverlay>(*this, 0, "That save does not exist!");
                            }
                        }
                        else {
                            currentOverlay = std::make_shared<ErrorOverlay>(*this, std::get<1>(result), std::get<2>(result));
                        }
                    }
                    else {
                        auto result = io::copyDirectory(title.fullPath(this->index(CELLS)) + "/", TEMP_PATH + "/");
                        if (result == 0) {
                            //I know you're not supposed to copy and paste code, but I don't really care right now.
                            this->index(CELLS, 0);
                            g_backupScrollEnabled = false;
                            entryType(TITLES);
                            if (io::fileExists(TEMP_PATH + "/main")) {
                                changeScreen(std::make_unique<SaveMainScreen>(title, TEMP_PATH + "/main"));
                            } else {
                                currentOverlay = std::make_shared<ErrorOverlay>(*this, 0, "That save does not exist!");
                            }
                        }
                        else {
                            currentOverlay = std::make_shared<ErrorOverlay>(*this, 0, "External save failed to copy \nto temporary location.");
                        }
                    }
                },
                [this]() { this->removeOverlay(); });
        }
        else {
            g_backupScrollEnabled = true;
            updateButtons();
            entryType(CELLS);
        }
    }

    // Handle pressing B
    if ((kdown & HidNpadButton_B) || (input.touch.count > 0 && input.touch.touches[0].x <= 532 && input.touch.touches[0].y <= 664)) {
        this->index(CELLS, 0);
        g_backupScrollEnabled = false;
        entryType(TITLES);
        updateButtons(); // Do this last
    }

    // Handle pressing/touching L
    if (buttonBackup->released() || (kdown & HidNpadButton_L)) {
        if (pksmBridge) {
            if (this->index(CELLS) != 0) {
                currentOverlay = std::make_shared<YesNoOverlay>(
                    *this, "Send save to PKSM 3DS?",
                    [this]() {
                        auto result = sendToPKSMBrigde(this->index(TITLES), g_currentUId, this->index(CELLS));
                        if (std::get<0>(result)) {
                            currentOverlay = std::make_shared<InfoOverlay>(*this, std::get<2>(result));
                        }
                        else {
                            currentOverlay = std::make_shared<ErrorOverlay>(*this, std::get<1>(result), std::get<2>(result));
                        }
                    },
                    [this]() { this->removeOverlay(); });
            }
        }
    }

    // Handle pressing/touching R
    if (buttonRestore->released() || (kdown & HidNpadButton_R)) {
        if (g_backupScrollEnabled) {
            if (pksmBridge && this->index(CELLS) != 0) {
                currentOverlay = std::make_shared<YesNoOverlay>(
                    *this, "Receive save from PKSM 3DS?",
                    [this]() {
                        auto result = recvFromPKSMBridge(this->index(TITLES), g_currentUId, this->index(CELLS));
                        if (std::get<0>(result)) {
                            currentOverlay = std::make_shared<InfoOverlay>(*this, std::get<2>(result));
                        }
                        else {
                            currentOverlay = std::make_shared<ErrorOverlay>(*this, std::get<1>(result), std::get<2>(result));
                        }
                    },
                    [this]() { this->removeOverlay(); });
            }
        }
    }
}

std::string MainScreen::nameFromCell(size_t index) const
{
    return backupList->cellName(index);
}

void MainScreen::entryType(entryType_t type_)
{
    type = type_;
}

void MainScreen::resetIndex(entryType_t type)
{
    if (type == TITLES) {
        hid.reset();
    }
    else {
        backupList->resetIndex();
    }
}

size_t MainScreen::index(entryType_t type) const
{
    return type == TITLES ? hid.fullIndex() : backupList->index();
}

void MainScreen::index(entryType_t type, size_t i)
{
    if (type == TITLES) {
        hid.page(i / hid.maxVisibleEntries());
        hid.index(i - hid.page() * hid.maxVisibleEntries());
    }
    else {
        backupList->setIndex(i);
    }
}

void MainScreen::setPKSMBridgeFlag(bool f)
{
    pksmBridge = f;
    updateButtons();
}

void MainScreen::updateButtons(void)
{
    if (g_backupScrollEnabled) {
        buttonBackup->canChangeColorWhenSelected(true);
        buttonRestore->canChangeColorWhenSelected(true);
        buttonBackup->setColors(theme().c2, theme().c6);
        buttonRestore->setColors(theme().c2, theme().c6);
    }
    else {
        buttonBackup->setColors(theme().c2, theme().c6);
        buttonRestore->setColors(theme().c2, theme().c6);
    }
}

std::string MainScreen::sortMode() const
{
    switch (g_sortMode) {
        case SORT_LAST_PLAYED:
            return "Last played";
        case SORT_PLAY_TIME:
            return "Play time";
        case SORT_ALPHA:
            return "Alphabetical";
        default:
            break;
    }
    return "";
}
