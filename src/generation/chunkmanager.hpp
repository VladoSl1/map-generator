#pragma once

#include "pipeline/voronoi_diagram.hpp"
#include "core/math/point2d.hpp"
#include "core/math/aabb.hpp"

#include "core/config.hpp"

#include <unordered_map>


namespace generation
{
    struct Chunk
    {
        math::Point2Di chunkCoords;
        math::AABB bounds;

        Chunk(math::Point2Di chunkCoords)
            : chunkCoords(chunkCoords)
        {
            double minX = chunkCoords.x * config::generation::CHUNK_WIDTH;
            double minY = chunkCoords.y * config::generation::CHUNK_HEIGHT;
            bounds.x_bounds = { minX, minX + config::generation::CHUNK_WIDTH };
            bounds.y_bounds = { minY, minY + config::generation::CHUNK_HEIGHT };
        }

        pipeline::VoronoiDiagram voronoiDiagram;
    };

    class ChunkManager
    {

    public:

        ChunkManager(int worldSeed)
            : m_worldSeed(worldSeed)
        {
            for (int x = -1; x <= 1; ++x)
            {
                for (int y = -1; y <= 1; ++y)
                {
                    addChunk({x, y});
                }
            }
        }

        void addChunk(math::Point2Di chunkCoords);
        void removeChunk(math::Point2Di chunkCoords);


    private:
        uint64_t m_worldSeed;
        std::unordered_map<long, Chunk> chunks;

        Chunk* getChunk(math::Point2Di chunkCoords);

        /* We are using uint64_t to make this program platform independent
         * */
        uint64_t hashChunk(math::Point2Di chunkCoords) const;
    };

}
