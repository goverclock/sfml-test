#include <print>

#include "minesweeper/minesweeper.h"
#include "raylib.h"

namespace ms {

void run(const std::vector<PlayerInfo>& players_info) {
    std::println("got player info:");
    int ind = 0;
    for (const PlayerInfo& pi : players_info) {
        std::println("[{}] {} {}", ++ind, pi.nickname, pi.ip);
    }

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
}

};  // namespace ms
