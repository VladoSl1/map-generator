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

static constexpr ColorStop OCEAN_GRADIENT[] = {
    { 0.00f, { 140, 210, 230, 255 } },
    { 0.20f, {  40, 170, 210, 255 } },
    { 1.00f, {  10,  25,  80, 255 } },
};

static constexpr ColorStop LAND_GRADIENT[] = {
    { 0.00f, {  34, 139,  34, 255 } },
    { 0.25f, {  90, 180,  60, 255 } },
    { 0.50f, { 160, 140,  80, 255 } },
    { 0.75f, { 100, 100, 100, 255 } },
    { 1.00f, { 255, 255, 255, 255 } },
};

static Color sampleGradient(float t, const ColorStop* stops, size_t count)
{
    t = std::clamp(t, 0.0f, 1.0f);

    if (t <= stops[0].position)         return stops[0].color;
    if (t >= stops[count - 1].position) return stops[count - 1].color;

    for (size_t i = 0; i + 1 < count; ++i)
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
    return stops[count - 1].color;
}

template <size_t N>
static Color sampleGradient(float t, const ColorStop (&stops)[N])
{
    return sampleGradient(t, stops, N);
}

static Color getContinuousColor(float elevation, float moisture, float baseTemperature)
{
    if (elevation < 0.0f)
    {
        float depth = std::clamp(-elevation * 2.0f, 0.0f, 1.0f);
        float currentVariation = moisture * 0.15f;
        float perceivedDepth = std::clamp(depth + currentVariation, 0.0f, 1.0f);

        return sampleGradient(elevation, OCEAN_GRADIENT);
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

    // static Color getContinuousColor(float elevation, float moisture, float baseTemperature)
    // {
    //     auto lerp = [](float a, float b, float t) { return a + t * (b - a); };
    //
    //     // OCEAN
    //     if (elevation < 0.0f)
    //     {
    //         float depth = std::clamp(-elevation * 2.0f, 0.0f, 1.0f);
    //         float currentVariation = moisture * 0.15f;
    //         float perceivedDepth = std::clamp(depth + currentVariation, 0.0f, 1.0f);
    //
    //         float r, g, b;
    //
    //         if (perceivedDepth < 0.2f)
    //         {
    //             float t = perceivedDepth / 0.2f;
    //             r = lerp(140.0f, 40.0f, t);
    //             g = lerp(210.0f, 170.0f, t);
    //             b = lerp(230.0f, 210.0f, t);
    //         }
    //         else
    //         {
    //             float t = (perceivedDepth - 0.2f) / 0.8f;
    //             r = lerp(40.0f, 10.0f, t);
    //             g = lerp(170.0f, 25.0f, t);
    //             b = lerp(210.0f, 80.0f, t);
    //         }
    //
    //         return {
    //             static_cast<unsigned char>(std::clamp(r, 0.0f, 255.0f)),
    //             static_cast<unsigned char>(std::clamp(g, 0.0f, 255.0f)),
    //             static_cast<unsigned char>(std::clamp(b, 0.0f, 255.0f)),
    //             255
    //         };
    //     }
    //
    //     // INLAND LAKES
    //     // if (moisture > 0.99f)
    //     // {
    //     //     float lakeDepth = std::clamp((moisture - 0.5f) * 2.0f, 0.0f, 1.0f);
    //     //     float r = lerp(60.0f, 20.0f, lakeDepth);
    //     //     float g = lerp(180.0f, 120.0f, lakeDepth);
    //     //     float b = lerp(240.0f, 200.0f, lakeDepth);
    //     //     return { static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b), 255 };
    //     // }
    //
    //     // LAND
    //     float t = std::clamp(elevation, 0.0f, 1.0f);
    //
    //     // if (t > 0.25f)
    //     // {
    //     //     float highElevationFactor = (t - 0.25f) / 0.75f;
    //     //
    //     //     float tempModifier = baseTemperature * 0.3f * highElevationFactor;
    //     //
    //     //     t = std::clamp(t - tempModifier, 0.0f, 1.0f);
    //     // }
    //
    //     float finalR, finalG, finalB;
    //
    //     if (t < 0.25f)
    //     {
    //         // Lowlands & Plains: Rich Green to Lighter Green
    //         float localT = t / 0.25f;
    //         finalR = lerp(34.0f, 90.0f, localT);
    //         finalG = lerp(139.0f, 180.0f, localT);
    //         finalB = lerp(34.0f, 60.0f, localT);
    //     }
    //     else if (t < 0.5f)
    //     {
    //         // Foothills: Lighter Green transitioning to Earthy Brown
    //         float localT = (t - 0.25f) / 0.25f;
    //         finalR = lerp(90.0f, 160.0f, localT);
    //         finalG = lerp(180.0f, 140.0f, localT);
    //         finalB = lerp(60.0f, 80.0f, localT);
    //     }
    //     else if (t < 0.75f)
    //     {
    //         // Mountains: Earthy Brown transitioning to Dark Grey/Stone
    //         float localT = (t - 0.5f) / 0.25f;
    //         finalR = lerp(160.0f, 100.0f, localT);
    //         finalG = lerp(140.0f, 100.0f, localT);
    //         finalB = lerp(80.0f, 100.0f, localT);
    //     }
    //     else
    //     {
    //         // Alpine Peaks: Stone transitioning to Snow
    //         float localT = (t - 0.75f) / 0.25f;
    //         finalR = lerp(100.0f, 255.0f, localT);
    //         finalG = lerp(100.0f, 255.0f, localT);
    //         finalB = lerp(100.0f, 255.0f, localT);
    //     }
    //
    //     return {
    //         static_cast<unsigned char>(std::clamp(finalR, 0.0f, 255.0f)),
    //         static_cast<unsigned char>(std::clamp(finalG, 0.0f, 255.0f)),
    //         static_cast<unsigned char>(std::clamp(finalB, 0.0f, 255.0f)),
    //         255
    //     };
    // }


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

            // Color cellColor = getBiomeColor(biomes[i]);
            Color cellColor = getContinuousColor(chunk.terrainData.elevationMap[i],
                                                chunk.terrainData.moistureMap[i],
                                                chunk.terrainData.temperatureMap[i]);

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
