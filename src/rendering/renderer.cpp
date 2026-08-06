#include "renderer.hpp"

#include "raylib_adapters.hpp"

#include "generation/chunkmanager.hpp"

#include "core/config.hpp"
#include "core/math/point2d.hpp"
#include "core/math/topology.hpp"

#include "core/utils/debug.hpp"

#include <vector>


namespace renderer
{
<<<<<<< HEAD
=======





>>>>>>> 2c8d59f (Fix missing return and do better decomposition)
    void renderPoints(const std::vector<math::Point2Dd>& points, Color color)
    {
        for (const auto& point : points)
        {
            DrawCircle(static_cast<int>(point.x),
                       static_cast<int>(point.y), config::renderer::POINT_RADIUS, color);
        }
    }

    void renderTriangles(const std::vector<math::Point2Dd>& points,
                         const std::vector<math::TriangleI>& triangles,
                         Color color)
    {
        for (const auto& triangle : triangles)
        {
            DrawTriangleLines(toRaylib(points[triangle[0]]),
                              toRaylib(points[triangle[1]]),
                              toRaylib(points[triangle[2]]),
                              color);
        }
    }

    void renderEdges(const std::vector<math::Point2Dd>& points,
                     const std::vector<math::EdgeI>& edges,
                     Color color)
    {
        for (const auto& edge : edges)
        {
            DrawLineV(toRaylib(points[edge[0]]),
                      toRaylib(points[edge[1]]),
                      color);
        }
    }

    void renderPolygons(const generation::pipeline::VoronoiDiagram& diagram,
                        Color color)
    {
        for (size_t i = 0; i < diagram.polygons.size(); ++i)
        {
            if (!diagram.isPolygonClosed(i))
            {
                log("Skipping open polygon at index ", i, " with ", diagram.polygons[i].indices.size(), " edges.");
                continue;
            }

            const auto& indices = diagram.polygons[i].indices;
            if (indices.size() < 3)
            {
                log("Skipping small polygon at index ", i, " with ", diagram.polygons[i].indices.size(), " edges.");
                continue;
            }

            std::vector<Vector2> fanPoints;
            fanPoints.reserve(indices.size());
            for (size_t idx : indices)
            {
                fanPoints.push_back(toRaylib(diagram.vertices[idx]));
            }

            DrawTriangleFan(fanPoints.data(), fanPoints.size(), color);

            fanPoints.push_back(fanPoints.front());
            DrawLineStrip(fanPoints.data(), fanPoints.size(), BLACK);
        }
    }

    void renderChunkGrid(const generation::ChunkManager& chunkManager)
    {
        const auto chunks = chunkManager.listAllChunks();

        for (const auto& chunk : chunks)
        {
            const auto startX = static_cast<float>(chunk->bounds.x_bounds.min);
            const auto startY = static_cast<float>(chunk->bounds.y_bounds.min);
            const auto width = static_cast<float>(chunk->bounds.x_bounds.length());
            const auto height = static_cast<float>(chunk->bounds.y_bounds.length());

            const Rectangle chunkRect{ startX, startY, width, height };

            // Draw the chunk border (2 pixels thick, gray color)
            DrawRectangleLinesEx(chunkRect, 2.0f, DARKGRAY);

            // Draw the chunk coordinates in the top-left corner of each chunk
            const char* coordText = TextFormat("(%d, %d)", chunk->chunkCoords.x, chunk->chunkCoords.y);
            DrawText(coordText, startX + 5, startY + 5, 20, LIGHTGRAY);
        }
    }
}
