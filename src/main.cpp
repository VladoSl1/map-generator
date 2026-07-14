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

    pipeline.generate();


    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // pipeline = generation::GenerationPipeline(config::window::WINDOW_WIDTH,
            //                                           config::window::WINDOW_HEIGHT,
            //                                           GetRandomValue(0, 2147483647));
            // pipeline.generate();
            auto newPoints = generation::pipeline::relaxVoronoiDiagram(pipeline.voronoiDiagram);
            pipeline.generateFromPoints(newPoints);
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            renderer::renderPoints(pipeline.voronoiDiagram.seeds, RED);
            renderer::renderPoints(pipeline.voronoiVertices, BLUE);
            // renderer::renderTriangles(pipeline.points, pipeline.triangles);
            renderer::renderEdges(pipeline.voronoiDiagram.vertices, pipeline.voronoiDiagram.edges, PURPLE);

            if (pipeline.voronoiDiagram.seeds.size() > 10)
            {
                renderer::renderPoints({
                    pipeline.voronoiDiagram.seeds[10]}, GREEN);

                std::vector<math::EdgeI> highlightedEdges;
                for (size_t edgeIdx : pipeline.voronoiDiagram.polygons[10].indices)
                {
                    highlightedEdges.push_back(pipeline.voronoiDiagram.edges[edgeIdx]);
                }

                // Pass the extracted edges to the existing renderer function
                renderer::renderEdges(pipeline.voronoiDiagram.vertices, highlightedEdges, ORANGE);
            }
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
