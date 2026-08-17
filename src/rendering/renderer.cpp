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

    struct ColorStop
    {
        float position;
        Color color;
    };

    static constexpr std::array<ColorStop, 3> OCEAN_GRADIENT = {{
        { 0.00f, { 140, 210, 230, 255 } },
        { 0.20f, {  40, 170, 210, 255 } },
        { 1.00f, {  10,  25,  80, 255 } },
    }};

    static constexpr std::array<ColorStop, 5> LAND_GRADIENT = {{
        { 0.00f, {  34, 139,  34, 255 } },
        { 0.25f, {  90, 180,  60, 255 } },
        { 0.50f, { 160, 140,  80, 255 } },
        { 0.75f, { 100, 100, 100, 255 } },
        { 1.00f, { 255, 255, 255, 255 } },
    }};

    template<size_t N>
    static Color sampleGradient(float t, const std::array<ColorStop, N> stops)
    {
        t = std::clamp(t, 0.0f, 1.0f);

        if (t <= stops[0].position)         return stops[0].color;
        if (t >= stops[N - 1].position) return stops[N - 1].color;

        for (size_t i = 0; i + 1 < N; ++i)
        {
            if (t <= stops[i + 1].position)
            {
                float localT = (t - stops[i].position)
                              / (stops[i + 1].position - stops[i].position);
                const Color& a = stops[i].color;
                const Color& b = stops[i + 1].color;

                return {
                    static_cast<unsigned char>(a.r + localT * (static_cast<float>(b.r) - a.r)),
                    static_cast<unsigned char>(a.g + localT * (static_cast<float>(b.g) - a.g)),
                    static_cast<unsigned char>(a.b + localT * (static_cast<float>(b.b) - a.b)),
                    255
                };
            }
        }
        return stops[N - 1].color;
    }

    // TODO: moisture and baseTemperature are not used in accordance with Wittakers diagram
    static Color getContinuousColor(float elevation, float moisture, float baseTemperature)
    {
        if (elevation < 0.0f)
        {
            float depth = std::clamp(-elevation * 2.0f, 0.0f, 1.0f);
            float currentVariation = moisture * 0.3f;
            float perceivedDepth = std::clamp(depth + currentVariation, 0.0f, 1.0f);

            return sampleGradient(-elevation, OCEAN_GRADIENT);
        }

        float t = std::clamp(elevation, 0.0f, 1.0f);

        // Snow line rises with temperature: warm regions need much higher actual
        // elevation before they read as alpine/snow-capped than cold ones do.
        if (t > config::renderer::SNOW_LINE_ELEVATION)
        {
            float highElevationFactor = (t - config::renderer::SNOW_LINE_ELEVATION)
                                       / (1.0f - config::renderer::SNOW_LINE_ELEVATION);
            float tempModifier = baseTemperature
                                * config::renderer::SNOW_LINE_TEMP_STRENGTH
                                * highElevationFactor;
            t = std::clamp(t - tempModifier, 0.0f, 1.0f);
        }

        return sampleGradient(t, LAND_GRADIENT);
    }

    void renderPoints(const std::vector<math::Point2Dd>& points, Color color, float radius)
    {
        for (const auto& point : points)
        {
            DrawCircle(static_cast<int>(point.x),
                       static_cast<int>(point.y), radius, color);
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
            const auto& indices = diagram.polygons[i].indices;

            if (!diagram.isPolygonClosed(i) || indices.size() < 3)
            {
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
            case terrain::BiomeType::DEEP_OCEAN:               return { 15, 45, 125, 255 };
            case terrain::BiomeType::OCEAN:                    return { 35, 105, 200, 255 };
            case terrain::BiomeType::LAKE:                     return { 60, 160, 240, 255 };

            case terrain::BiomeType::BEACH:                    return { 245, 225, 135, 255 };

            case terrain::BiomeType::SCORCHED:                 return { 85, 55, 45, 255 };
            case terrain::BiomeType::BARE:                     return { 145, 135, 125, 255 };
            case terrain::BiomeType::TUNDRA:                   return { 140, 170, 160, 255 };
            case terrain::BiomeType::SNOW:                     return { 240, 250, 255, 255 };

            case terrain::BiomeType::TEMPERATE_DESERT:         return { 225, 190, 130, 255 };
            case terrain::BiomeType::SHRUBLAND:                return { 145, 185, 115, 255 };
            case terrain::BiomeType::TAIGA:                    return { 45, 115, 75, 255 };

            case terrain::BiomeType::SUBTROPICAL_DESERT:       return { 240, 180, 100, 255 };
            case terrain::BiomeType::GRASSLAND:                return { 100, 200, 85, 255 };
            case terrain::BiomeType::TROPICAL_SEASONAL_FOREST: return { 65, 160, 55, 255 };
            case terrain::BiomeType::TROPICAL_RAIN_FOREST:     return { 20, 110, 40, 255 };

            default:                                           return MAGENTA;
        }
    }

    void renderChunk(const generation::Chunk& chunk)
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

            Color cellColor;
            if constexpr (config::renderer::DISCRETE_BIOME_COLORS)
            {
                cellColor = getBiomeColor(biomes[i]);
            }
            else
            {
                cellColor = getContinuousColor(chunk.terrainData.elevationMap[i],
                                                     chunk.terrainData.moistureMap[i],
                                                     chunk.terrainData.temperatureMap[i]);
            }

            std::vector<Vector2> fanPoints;
            fanPoints.reserve(diagram.polygons[i].indices.size());
            for (size_t idx : diagram.polygons[i].indices)
            {
                fanPoints.push_back(toRaylib(diagram.vertices[idx]));
            }

            DrawTriangleFan(fanPoints.data(), fanPoints.size(), cellColor);

            if constexpr (config::renderer::SHOW_POLYGON_OUTLINES)
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
