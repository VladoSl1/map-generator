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

            DrawCircle(
                math::getRandomInt(engine, 0, config::window::WINDOW_WIDTH),
                math::getRandomInt(engine, 0, config::window::WINDOW_HEIGHT),
                math::getRandomFloat(engine, 10.0f, 50.0f),
                Color{static_cast<unsigned char>(math::getRandomInt(engine, 0, 255)),
                      static_cast<unsigned char>(math::getRandomInt(engine, 0, 255)),
                      static_cast<unsigned char>(math::getRandomInt(engine, 0, 255)),
                      255}
            );
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
