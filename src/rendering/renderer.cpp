#include "renderer.hpp"

#include "core/config.hpp"

#include "raylib_adapters.hpp"

namespace renderer
{
    void renderPoints(const std::vector<math::Point2Dd>& points, Color color)
    {
        for (const auto& point : points)
        {
            DrawCircle(point.x, point.y, config::renderer::POINT_RADIUS, color);
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

    void renderChunkGrid(const generation::ChunkManager& chunkManager)
    {
        // Get all currently loaded chunks
        auto chunks = chunkManager.listAllChunks();

        for (const auto chunk : chunks)
        {
            // Calculate screen coordinates based on chunk coordinates and your config
            float startX = chunk->bounds.x_bounds.min;
            float startY = chunk->bounds.y_bounds.min;
            float width = chunk->bounds.x_bounds.length();
            float height = chunk->bounds.y_bounds.length();

            Rectangle chunkRect = { startX, startY, width, height };

            // Draw the chunk border (2 pixels thick, gray color)
            DrawRectangleLinesEx(chunkRect, 2.0f, DARKGRAY);

            // Draw the chunk coordinates in the top-left corner of each chunk
            const char* coordText = TextFormat("(%d, %d)", chunk->chunkCoords.x, chunk->chunkCoords.y);
            DrawText(coordText, startX + 5, startY + 5, 20, LIGHTGRAY);
        }
    }
}
