#include "raylib.h"
#include "core/config.hpp"

#include "generation/pipeline/pipeline.hpp"
#include "rendering/renderer.hpp"

int main()
{
    InitWindow(config::window::WINDOW_WIDTH,
               config::window::WINDOW_HEIGHT,
               config::window::TITLE
    );
    SetTargetFPS(config::window::FPS);

    generation::GenerationPipeline pipeline(config::window::WINDOW_WIDTH,
                                            config::window::WINDOW_HEIGHT,
                                            42);

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            pipeline = generation::GenerationPipeline(config::window::WINDOW_WIDTH,
                                                      config::window::WINDOW_HEIGHT,
                                                      GetRandomValue(0, 2147483647));
            pipeline.generate();
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            renderer::renderPoints(pipeline.points, RED);
            renderer::renderPoints(pipeline.voronoiVertices, BLUE);
            // renderer::renderTriangles(pipeline.points, pipeline.triangles);
            renderer::renderEdges(pipeline.voronoiDiagram.vertices, pipeline.voronoiDiagram.edges, PURPLE);

        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
