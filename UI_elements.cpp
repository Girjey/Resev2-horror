// ui.cpp

#include "UI_elements.h"
#include <cstring> // for strlen

// --- Вспомогательная структура для generateActionButtons ---
struct RowInfo {
    int startIndex;
    int endIndex;
    float totalOriginalButtonWidth;
    int buttonCount;
};

void DrawTextRec(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint) {
    float textOffsetX = rec.x;
    float textOffsetY = rec.y;
    int letter = 0;

    if (wordWrap)
    {
        int currentLine = 0;
        Vector2 spaceSize = MeasureTextEx(font, " ", fontSize, spacing);
        float wordSpacing = spaceSize.x;

        while (letter < strlen(text))
        {
            int wordEnd = letter;
            while ((wordEnd < strlen(text)) && (text[wordEnd] != ' ') && (text[wordEnd] != '\n')) wordEnd++;

            Vector2 wordSize = MeasureTextEx(font, TextSubtext(text, letter, wordEnd - letter), fontSize, spacing);
            float wordWidth = wordSize.x;
            float wordHeight = wordSize.y;

            if ((textOffsetX + wordWidth) > (rec.x + rec.width) && (textOffsetX != rec.x))
            {
                currentLine++;
                textOffsetX = rec.x;
                textOffsetY = rec.y + currentLine * (wordHeight + spacing);
            }

            if (text[wordEnd] == '\n')
            {
                currentLine++;
                textOffsetX = rec.x;
                textOffsetY = rec.y + currentLine * (wordHeight + spacing);
            }

            Vector2 drawPos = { textOffsetX, textOffsetY };
            DrawTextEx(font, TextSubtext(text, letter, wordEnd - letter), drawPos, fontSize, spacing, tint);

            textOffsetX += (wordWidth + wordSpacing);
            letter = wordEnd + 1;
        }
    }
    else
    {
        Vector2 drawPos = { rec.x, rec.y };
        DrawTextEx(font, text, drawPos, fontSize, spacing, tint);
    }
}

std::vector<ActionButton> generateActionButtons(
    const Room& currentRoom,
    const Rectangle& actionPanel,
    const Font& customFont)
{
    std::vector<std::string> actionNames;
    std::vector<ActionButton> tempButtons;
    std::vector<RowInfo> rowInfos;

    actionNames.push_back("Look around");
    actionNames.push_back("Use item");

    for (const auto& pair : currentRoom.exits) {
        actionNames.push_back(pair.first);
    }

    for (const auto& pair : currentRoom.interactables) {
        if (pair.second) {
            actionNames.push_back("Interact " + pair.first);
        }
    }

    const int btnHeight = 50;
    const int btnSpacing = 10;
    const float fontSize = 20;
    const float fontSpacing = 2.0f;
    const float paddingX = 40.0f;

    int currentRowStartIndex = 0;
    float currentX = actionPanel.x + btnSpacing;
    int currentRowButtonCount = 0;
    float currentRowTotalOriginalWidth = 0.0f;
    int maxRows = 2;
    int currentRowIndex = 0;
    float startY = actionPanel.y + btnSpacing;

    for (const std::string& actionName : actionNames) {
        Vector2 textSize = MeasureTextEx(customFont, actionName.c_str(), fontSize, fontSpacing);
        float originalBtnWidth = textSize.x + paddingX;
        float requiredTotalWidth = originalBtnWidth + btnSpacing;

        if (currentX + requiredTotalWidth > actionPanel.x + actionPanel.width) {
            if (currentRowButtonCount > 0) {
                rowInfos.push_back({ currentRowStartIndex, (int)tempButtons.size() - 1, currentRowTotalOriginalWidth, currentRowButtonCount });
                currentRowIndex++;
            }

            if (currentRowIndex >= maxRows) break;

            currentRowStartIndex = tempButtons.size();
            currentRowButtonCount = 0;
            currentRowTotalOriginalWidth = 0.0f;
            currentX = actionPanel.x + btnSpacing;
            startY += btnHeight + btnSpacing;
        }

        tempButtons.push_back({ { currentX, startY, originalBtnWidth, (float)btnHeight }, actionName, false });

        currentX += requiredTotalWidth;
        currentRowButtonCount++;
        currentRowTotalOriginalWidth += originalBtnWidth;
    }

    if (currentRowButtonCount > 0) {
        rowInfos.push_back({ currentRowStartIndex, (int)tempButtons.size() - 1, currentRowTotalOriginalWidth, currentRowButtonCount });
    }

    std::vector<ActionButton> finalButtons;
    const float availableRowWidth = actionPanel.width - 2 * btnSpacing;

    for (const auto& row : rowInfos) {
        float internalSpacingWidth = (row.buttonCount > 0 ? (row.buttonCount - 1) : 0) * btnSpacing;
        float occupiedWidth = row.totalOriginalButtonWidth + internalSpacingWidth;
        float freeSpace = availableRowWidth - occupiedWidth;
        float extraWidthPerButton = (row.buttonCount > 0) ? freeSpace / row.buttonCount : 0;

        float currentXPosition = actionPanel.x + btnSpacing;

        for (int i = row.startIndex; i <= row.endIndex; i++) {
            ActionButton btn = tempButtons[i];
            float newWidth = btn.rect.width + extraWidthPerButton;
            btn.rect.x = currentXPosition;
            btn.rect.width = newWidth;
            currentXPosition += newWidth + btnSpacing;
            finalButtons.push_back(btn);
        }
    }

    return finalButtons;
}