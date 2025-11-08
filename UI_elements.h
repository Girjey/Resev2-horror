#ifndef UI_H
#define UI_H

#include "structs.h"

void DrawTextRec(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);

std::vector<ActionButton> generateActionButtons(
    const Room& currentRoom,
    const Rectangle& actionPanel,
    const Font& customFont
);

#endif // UI_H