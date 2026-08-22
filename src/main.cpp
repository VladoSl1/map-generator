#include "raylib.h"

#include "generation/pipeline/pipeline.hpp"
#include "generation/pipeline/voronoi_diagram.hpp"

#include "generation/chunkmanager.hpp"

#include "rendering/renderer.hpp"
#include "rendering/camera_control.hpp"
#include "rendering/chunkstreamer.hpp"

#include "core/config.hpp"
#include "core/utils/debug.hpp"
#include "core/utils/random.hpp"

#include <vector>
#include <limits>


int main()
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(config::window::WINDOW_WIDTH,
               config::window::WINDOW_HEIGHT,
               config::window::TITLE
    );
    SetTargetFPS(config::window::FPS);

    const math::AABB bounds{ {0, config::window::WINDOW_WIDTH}, {0, config::window::WINDOW_HEIGHT} };

    int seed = config::generation::SEED;
    if constexpr (config::generation::SEED == 0)
    {
        seed = utils::getRandomNumber<int>({1, std::numeric_limits<int>::max()}, seed);
    }

    auto chunkManager = std::make_shared<generation::ChunkManager>(seed);

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
