#include "chunkmanager.hpp"

#include "core/config.hpp"

#include "generation/pipeline/pipeline.hpp"

namespace generation
{
    void ChunkManager::addChunk(math::Point2Di chunkCoords)
    {
        uint64_t key = hashChunk(chunkCoords);

        if (chunks.find(key) == chunks.end())
        {
            Chunk newChunk(chunkCoords);

            newChunk.voronoiDiagram = pipeline::generate(
                hashChunk(chunkCoords),
                newChunk.bounds
            );

            chunks.emplace(key, std::move(newChunk));
        }
    }


    void ChunkManager::removeChunk(math::Point2Di chunkCoords)
    {

    }


    Chunk* ChunkManager::getChunk(math::Point2Di chunkCoords)
    {
        long key = hashChunk(chunkCoords);

        auto it = chunks.find(key);
        if (it != chunks.end())
        {
            return &(it->second);
        }
        else
        {
            return nullptr;
        }
    }

    void ChunkManager::preloadChunks(math::Point2Di centerChunkCoords)
    {
        // we are preloading 3x3 chunks around the center chunk
        for (int x = -1; x <= 1; ++x)
        {
            for (int y = -1; y <= 1; ++y)
            {
                addChunk({centerChunkCoords.x + x, centerChunkCoords.y + y});
            }
        }
    }

    std::vector<const Chunk*> ChunkManager::listAllChunks() const
    {
        std::vector<const Chunk*> chunkList;
        for (const auto& pair : chunks)
        {
            chunkList.push_back(&(pair.second));
        }

        return chunkList;
    }

    uint64_t ChunkManager::hashChunk(math::Point2Di chunkCoords) const
    {
        // cast to uint32_t first to drop the sign-extension behavior
        uint32_t ux = static_cast<uint32_t>(chunkCoords.x);
        uint32_t uy = static_cast<uint32_t>(chunkCoords.y);

        uint64_t seed = (static_cast<uint64_t>(ux) << 32) | uy;

        seed ^= m_worldSeed;

        // alg: https://rosettacode.org/wiki/Pseudo-random_numbers/Splitmix64
        seed += 0x9e3779b97f4a7c15;
        seed = (seed ^ (seed >> 30)) * 0xbf58476d1ce4e5b9;
        seed = (seed ^ (seed >> 27)) * 0x94d049bb133111eb;
        return seed ^ (seed >> 31);
    }
}
