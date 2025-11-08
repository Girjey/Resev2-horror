#include "raylib.h"

int main() {
    InitWindow(800, 600, "Русский текст в Raylib");
    SetTargetFPS(60);

    // Загружаем шрифт (используем твой OpenSans)
    Font font = LoadFontEx("OpenSans-VariableFont_wdth,wght.ttf", 48, 0, 0);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextEx(font, "Привет, Raylib!", { 200, 280 }, 48, 1, BLACK);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
}
