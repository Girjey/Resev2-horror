#include "raylib.h"

int main() {
    InitWindow(800, 600, "Program Raylib");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, Raylib!", 300, 280, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
}
