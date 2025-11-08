// main.cpp

#include "raylib.h"
#include "structs.h"
#include "gameData.h"
#include "game_logic.h"
#include "UI_elements.h"

int main() {
    const int TYPUNG_SPEED_FRAMES = 3;
    static int frame_counter = 0;
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Interface Prototype");

    Font customFont = LoadFont("VCR_OSD_MONO_1.001.ttf");
    SetTargetFPS(60);

    // --- Game Data Initialization ---
    std::map<LocationID, Room> gameMap = initializeGameMap();
    std::map<ItemType, Item> itemDatabase = initializeItemDatabase();
    GameState gameState;

    gameState.current_room_id = LocationID::MAIN_HALL;
    gameState.setSceneText(gameMap.at(gameState.current_room_id).description);

    // --- UI Layout ---
    Rectangle minimap = { 0, (float)880, (float)300, (float)200 };
    Rectangle inventoryArea = { 0, 0, (float)300, (float)880 };
    Rectangle actionPanel = { (float)300, (float)880, (float)1620, (float)200 };
    Rectangle mainScene = { (float)300, 0, (float)1620, (float)880 };

    // Inventory Slots
    const int invRows = 4;
    const int slotSpacing = 10;
    const int slotSize = 100;
    std::vector<InventorySlot> inventory;
    for (int i = 0; i < invRows; i++) {
        InventorySlot slot;
        slot.rect = {
            inventoryArea.x + slotSpacing,
            inventoryArea.y + slotSpacing + i * (slotSize + slotSpacing),
            (float)slotSize,
            (float)slotSize
        };
        inventory.push_back(slot);
    }

    std::vector<ActionButton> buttons = generateActionButtons(
        gameMap.at(gameState.current_room_id),
        actionPanel,
        customFont
    );

    // --- Main Game Loop ---
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        std::string chosen_action = "";

        if (!gameState.is_typing_finished)
        {
            frame_counter++;

            if (frame_counter >= TYPUNG_SPEED_FRAMES)
            {
                if (gameState.visible_chars_count < gameState.scenes.length()) 
                {
                    gameState.visible_chars_count++;
                }
                else
                {
                    frame_counter = 0;
                }
            }
        }

        // 1. UI Interaction: Buttons
        for (auto& btn : buttons) {
            btn.hovered = CheckCollisionPointRec(mousePos, btn.rect);
            if (btn.hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                chosen_action = btn.text;
                break;
            }
        }

        // 2. UI Interaction: Inventory
        for (auto& slot : inventory) {
            slot.hovered = CheckCollisionPointRec(mousePos, slot.rect);
            if (slot.hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                int index = &slot - &inventory[0];
                gameState.setSceneText("Clicked inventory slot " + std::to_string(index) + "!");
            }
        }

        // 3. Process Game Action
        if (!chosen_action.empty()) {
            handlePlayerAction(chosen_action, gameMap, gameState, itemDatabase);
            buttons = generateActionButtons(gameMap.at(gameState.current_room_id), actionPanel, customFont);
        }

        // --- DRAWING ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Main Scene
        std::string visibleText = gameState.scenes.substr(0, gameState.visible_chars_count);
        DrawRectangleRec(mainScene, LIGHTGRAY);
        Rectangle textBounds = { mainScene.x + 20, mainScene.y + 60, mainScene.width - 15, mainScene.height - 80 };
        DrawTextRec(customFont,visibleText.c_str(), textBounds, 30, 2.0f, true, BLACK);
        DrawRectangleLinesEx(mainScene, 2, BLACK);

        // Inventory Area
        DrawRectangleRec(inventoryArea, SKYBLUE);
        for (size_t i = 0; i < inventory.size(); ++i) {
            DrawRectangleRec(inventory[i].rect, BLUE);
            if (i < gameState.inventory.size()) {
                const auto& item = itemDatabase.at(gameState.inventory[i]);
                DrawTextEx(customFont, item.name.c_str(), { inventory[i].rect.x + 10, inventory[i].rect.y + 10 }, 20, 2, WHITE);
            }
            if (inventory[i].hovered) DrawRectangleLinesEx(inventory[i].rect, 4, RED);
        }

        // Minimap
        DrawRectangleRec(minimap, DARKGREEN);
        DrawTextEx(customFont, "Minimap", { minimap.x + 20, minimap.y + 20 }, 20, 2, WHITE);

        // Action Panel
        DrawRectangleRec(actionPanel, ORANGE);

        // Draw Action Buttons
        for (auto& btn : buttons) {
            DrawRectangleRec(btn.rect, DARKGRAY);
            Vector2 textSize = MeasureTextEx(customFont, btn.text.c_str(), 20, 2);
            DrawTextEx(customFont, btn.text.c_str(),
                { btn.rect.x + (btn.rect.width - textSize.x) / 2, btn.rect.y + (btn.rect.height - textSize.y) / 2 },
                20, 2, WHITE);
            if (btn.hovered) DrawRectangleLinesEx(btn.rect, 3, RED);
        }

        EndDrawing();
    }
    UnloadFont(customFont);
    CloseWindow();
    return 0;
}