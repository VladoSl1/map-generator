#pragma once

#include "pipeline/voronoi_diagram.hpp"
#include "core/math/point2d.hpp"
#include "core/math/aabb.hpp"
#include "core/config.hpp"

#include <unordered_map>
#include <vector>
#include <memory>

namespace generation
{
    struct Chunk
    {
        enum class State
        {
            UNLOADED,
            PRELOADED,
            LOADED
        };

        math::Point2Di chunkCoords;
        math::AABB bounds;

        std::vector<math::Point2Dd> preloadedSeeds;
        pipeline::VoronoiDiagram voronoiDiagram; //TODO: consider using pointer to seeds in voronoiDiagram to avoid copying seeds when loading chunks
        State state = State::UNLOADED;

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
            //TODO: for testing purposes
            for (int x = -1; x <= 1; ++x)
            {
                for (int y = -1; y <= 1; ++y)
                {
                    loadChunk({x, y});
                }
            }
        }

        void loadChunk(math::Point2Di chunkCoords);
        void removeChunk(math::Point2Di chunkCoords);

        std::shared_ptr<Chunk> addChunk(math::Point2Di chunkCoords);
        std::shared_ptr<Chunk> getChunk(math::Point2Di chunkCoords) const;
        std::shared_ptr<Chunk> requestChunk(math::Point2Di chunkCoords);

        void preloadChunk(math::Point2Di chunkCoords);
        void preloadChunks(math::Point2Di centerChunkCoords);
        std::vector<std::shared_ptr<Chunk>> listAllChunks() const;

    private:
        uint64_t m_worldSeed;
        std::unordered_map<uint64_t, std::shared_ptr<Chunk>> chunks; // TODO: consider using unique_ptr

        /* We are using uint64_t to have guaranteed 64-bit hash values */
        uint64_t hashChunk(math::Point2Di chunkCoords) const;
    };
}
