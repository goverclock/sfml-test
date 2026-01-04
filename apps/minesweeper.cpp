#include "raylib.h"

int main() {
    const int width = 800;
    const int height = 450;

    InitWindow(width, height, "the minesweeper");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("love raylib", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
