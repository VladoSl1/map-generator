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

    static Color getBiomeColor(terrain::BiomeType biome)
    {
        switch (biome)
        {
            case terrain::BiomeType::DEEP_OCEAN:               return { 26, 68, 122, 255 };
            case terrain::BiomeType::OCEAN:                    return { 66, 161, 204, 255 };

            case terrain::BiomeType::BEACH:                    return { 240, 223, 163, 255 };

            case terrain::BiomeType::SCORCHED:                 return { 71, 59, 58, 255 };
            case terrain::BiomeType::BARE:                     return { 130, 119, 112, 255 };
            case terrain::BiomeType::TUNDRA:                   return { 166, 184, 173, 255 };
            case terrain::BiomeType::SNOW:                     return { 245, 250, 252, 255 };

            case terrain::BiomeType::TEMPERATE_DESERT:         return { 214, 203, 156, 255 };
            case terrain::BiomeType::SHRUBLAND:                return { 156, 176, 130, 255 };
            case terrain::BiomeType::TAIGA:                    return { 106, 140, 107, 255 };

            case terrain::BiomeType::SUBTROPICAL_DESERT:       return { 224, 194, 137, 255 };
            case terrain::BiomeType::GRASSLAND:                return { 128, 191, 117, 255 };
            case terrain::BiomeType::TROPICAL_SEASONAL_FOREST: return { 89, 168, 92, 255 };
            case terrain::BiomeType::TROPICAL_RAIN_FOREST:     return { 44, 130, 81, 255 };

            default:                                           return MAGENTA; // Error color
        }
    }

    void renderChunk(const generation::Chunk& chunk, bool polygonOutlines)
    {
        const auto& diagram = chunk.voronoiDiagram;
        const auto& biomes = chunk.terrainData.biomes;

        for (size_t i = 0; i < diagram.polygons.size(); ++i)
        {
            // Skip invalid polygons
            if (!diagram.isPolygonClosed(i) || diagram.polygons[i].indices.size() < 3)
            {
                continue;
            }

            Color cellColor = getBiomeColor(biomes[i]);

            std::vector<Vector2> fanPoints;
            fanPoints.reserve(diagram.polygons[i].indices.size());
            for (size_t idx : diagram.polygons[i].indices)
            {
                fanPoints.push_back(toRaylib(diagram.vertices[idx]));
            }

            DrawTriangleFan(fanPoints.data(), fanPoints.size(), cellColor);

            if (polygonOutlines)
            {
                fanPoints.push_back(fanPoints.front());
                DrawLineStrip(fanPoints.data(), fanPoints.size(), ColorAlpha(BLACK, 0.1f));
            }
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

            DrawRectangleLinesEx(chunkRect, 2.0f, DARKGRAY);

            const char* coordText = TextFormat("(%d, %d)", chunk->chunkCoords.x, chunk->chunkCoords.y);
            DrawText(coordText, startX + 5, startY + 5, 20, LIGHTGRAY);
        }
    }
}
