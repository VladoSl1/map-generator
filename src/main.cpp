#include "raylib.h"
#include "core/config.hpp"
#include "core/utils/random.hpp"

int main()
{
    InitWindow(config::window::WINDOW_WIDTH,
               config::window::WINDOW_HEIGHT,
               config::window::TITLE
    );
    SetTargetFPS(config::window::FPS);

    std::mt19937 engine(std::random_device{}());

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);



        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
