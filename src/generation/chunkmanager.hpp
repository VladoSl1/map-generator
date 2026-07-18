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

        pipeline::VoronoiDiagram voronoiDiagram;

        Chunk(math::Point2Di chunkCoords);

        static math::AABB getBounds(math::Point2Di chunkCoords);
    };

    class ChunkManager
    {

    public:

        ChunkManager(int worldSeed)
            : m_worldSeed(worldSeed)
        {
            preloadChunks({0, 0});
            addChunk({0, 0});

        }

        void addChunk(math::Point2Di chunkCoords);
        void removeChunk(math::Point2Di chunkCoords);

        Chunk* getChunk(math::Point2Di chunkCoords);

        void preloadChunks(math::Point2Di centerChunkCoords);
        std::vector<const Chunk*> listAllChunks() const;


    private:
        uint64_t m_worldSeed;
        std::unordered_map<uint64_t, Chunk> chunks;

        /* We are using uint64_t to make this program platform independent
         * */
        uint64_t hashChunk(math::Point2Di chunkCoords) const;
    };

}
