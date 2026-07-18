#include "raylib.h"

#include "core/config.hpp"

#include "generation/pipeline/pipeline.hpp"
#include "generation/pipeline/voronoi_diagram.hpp"

#include "generation/chunkmanager.hpp"

#include "rendering/renderer.hpp"
#include "rendering/camera_control.hpp"

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

    log("Generating chunks...");
    generation::ChunkManager chunkManager(42);
    log("Generating chunks... done");

    log("Picking Voronoi diagram...");
    auto chunks = chunkManager.listAllChunks();
    // auto voronoiDiagram = chunkManager.listAllChunks()[0]->voronoiDiagram;
    auto voronoiDiagram = chunkManager.getChunk({0, 0})->voronoiDiagram;
    log("Picking Voronoi diagram... done");


    renderer::CameraController cameraController(config::window::WINDOW_WIDTH, config::window::WINDOW_HEIGHT);

    while (!WindowShouldClose())
    {
        cameraController.update();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // voronoiDiagram = generation::pipeline::generate(
            //     GetRandomValue(0, 2147483647),
            //     bounds
            // );

            // auto newPoints = generation::pipeline::relaxVoronoiDiagram(voronoiDiagram);
            // voronoiDiagram = generation::pipeline::generateFromPoints(newPoints);
            //
            config::generation::RELAXATION_ITERATIONS++; //TODO: remove this, just for testing

            chunkManager = generation::ChunkManager(42);
            chunks = chunkManager.listAllChunks();
            voronoiDiagram = chunkManager.getChunk({0, 0})->voronoiDiagram;
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode2D(cameraController.getCamera());
            {
                for (const auto& chunk : chunks)
                {
                    voronoiDiagram = chunk->voronoiDiagram;
                    renderer::renderPoints(voronoiDiagram.seeds, RED);
                    renderer::renderPoints(voronoiDiagram.vertices, BLUE);
                    renderer::renderEdges(voronoiDiagram.vertices, voronoiDiagram.edges, PURPLE);

                    if (voronoiDiagram.seeds.size() > 10)
                    {
                        renderer::renderPoints({
                            voronoiDiagram.seeds[10]}, GREEN);

                        std::vector<math::EdgeI> highlightedEdges;
                        for (size_t edgeIdx : voronoiDiagram.polygons[10].indices)
                        {
                            highlightedEdges.push_back(voronoiDiagram.edges[edgeIdx]);
                        }

                        renderer::renderEdges(voronoiDiagram.vertices, highlightedEdges, ORANGE);
                    }
                }

                renderer::renderChunkGrid(chunkManager);
            }
            EndMode2D(); // End Camera Transformations

        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
