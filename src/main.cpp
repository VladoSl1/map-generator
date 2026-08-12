#include "raylib.h"

#include "core/config.hpp"

#include "generation/pipeline/pipeline.hpp"
#include "generation/pipeline/voronoi_diagram.hpp"

#include "generation/chunkmanager.hpp"

#include "rendering/renderer.hpp"
#include "rendering/camera_control.hpp"
#include "rendering/chunkstreamer.hpp"

#include <vector>

#include "core/utils/debug.hpp"


int main()
{
    InitWindow(config::window::WINDOW_WIDTH,
               config::window::WINDOW_HEIGHT,
               config::window::TITLE
    );
    SetTargetFPS(config::window::FPS);

    math::AABB bounds{ {0, config::window::WINDOW_WIDTH}, {0, config::window::WINDOW_HEIGHT} };

    auto chunkManager = std::make_shared<generation::ChunkManager>(42);

    auto chunks = chunkManager->listAllChunks();
    // auto voronoiDiagram = chunkManager.listAllChunks()[0]->voronoiDiagram;
    auto voronoiDiagram = chunkManager->getChunk({0, 0})->voronoiDiagram;

    renderer::CameraController cameraController(config::window::WINDOW_WIDTH, config::window::WINDOW_HEIGHT);
    renderer::ChunkStreamer chunkStreamer(chunkManager);

    while (!WindowShouldClose())
    {
        cameraController.update();

        chunkStreamer.updateLoadedChunks(cameraController.getViewBounds());

        chunks = chunkManager->listAllChunks();

        float baseTemperature = 0.0f;
        float moistureMultiplier = 0.0f;

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            // config::generation::RELAXATION_ITERATIONS++;
            //
            // chunkManager = std::make_shared<generation::ChunkManager>(42);
            //
            // chunks = chunkManager->listAllChunks();
            // voronoiDiagram = chunkManager->getChunk({0, 0})->voronoiDiagram;
            //
            // chunkStreamer = renderer::ChunkStreamer(chunkManager);
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);


            BeginMode2D(cameraController.getCamera());
            {
                for (const auto& chunk : chunks)
                {
                    voronoiDiagram = chunk->voronoiDiagram;
                     // renderer::renderPolygons(voronoiDiagram, LIGHTGRAY);

                    renderer::renderChunk(*chunk, false);

                    // renderer::renderPoints(voronoiDiagram.seeds, RED);
                    // renderer::renderPoints(voronoiDiagram.vertices, BLUE);
                    // renderer::renderEdges(voronoiDiagram.vertices, voronoiDiagram.edges, PURPLE);
                }

                // uncomment to show the chunk grid
                // renderer::renderChunkGrid(*chunkManager);
            }
            EndMode2D(); // End Camera Transformations
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
