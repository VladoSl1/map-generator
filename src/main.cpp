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
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
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

    renderer::CameraController cameraController;
    renderer::ChunkStreamer chunkStreamer(chunkManager);

    while (!WindowShouldClose())
    {
        cameraController.update();

        chunkStreamer.updateLoadedChunks(cameraController.getViewBounds());

        chunks = chunkManager->listAllChunks();

        float baseTemperature = 0.0f;
        float moistureMultiplier = 0.0f;

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);


            BeginMode2D(cameraController.getCamera());
            {
                for (const auto& chunk : chunks)
                {
                    voronoiDiagram = chunk->voronoiDiagram;
                     // renderer::renderPolygons(voronoiDiagram, LIGHTGRAY);

                    renderer::renderChunk(*chunk);

                    // renderer::renderPoints(voronoiDiagram.seeds, RED);
                    // renderer::renderPoints(voronoiDiagram.vertices, BLUE);
                    // renderer::renderEdges(voronoiDiagram.vertices, voronoiDiagram.edges, PURPLE);
                }

                if (config::renderer::SHOW_CHUNK_GRID)
                {
                    renderer::renderChunkGrid(*chunkManager);
                }
            }
            EndMode2D();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
