#include "raylib.h"

#include "core/config.hpp"

#include "generation/pipeline/pipeline.hpp"
#include "generation/pipeline/voronoi_diagram.hpp"

#include "rendering/renderer.hpp"

#include <vector>


int main()
{
    InitWindow(config::window::WINDOW_WIDTH,
               config::window::WINDOW_HEIGHT,
               config::window::TITLE
    );
    SetTargetFPS(config::window::FPS);

    math::AABB bounds{ {0, config::window::WINDOW_WIDTH}, {0, config::window::WINDOW_HEIGHT} };

    generation::pipeline::VoronoiDiagram voronoiDiagram = generation::pipeline::generate(
        42,
        bounds
    );

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            voronoiDiagram = generation::pipeline::generate(
                GetRandomValue(0, 2147483647),
                bounds
            );

            // auto newPoints = generation::pipeline::relaxVoronoiDiagram(voronoiDiagram);
            // voronoiDiagram = generation::pipeline::generateFromPoints(newPoints);
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

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
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
